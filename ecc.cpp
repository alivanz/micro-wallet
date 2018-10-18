#include "ecc.hpp"
#include "command.hpp"
#include "common.hpp"
#include "storage.hpp"

#include <uECC.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

string SetPrivateKey::Help(){ return "WARNING: maybe unsafe! Set private key (int index, string curve, hex privkey) => (status)";}
std::map<string,string> SetPrivateKey::Call(std::map<string,string> data){
  // Parameter
  int index            = atoi(GetValue(data, "index").c_str());
  string scurve        = GetValue(data, "curve");
  vector<char> privkey = HexToBytes(GetValue(data, "privkey"));
  // Check Curve
  if(scurve!="secp256k1"){
    throw "unsupported curve";
  }
  // Set private key
  Storage::SetBlock(index, privkey);
  // Result
  std::map<string,string> resp;
  // resp["index"] = ssprintf("%i", index);
  // resp["curve"] = scurve;
  // resp["privkey"] = BytesToHex(privkey);
  return resp;
}

string GetPubkey::Help(){ return "Get public key (int index) => (curve, pubkey)"; }
std::map<string,string> GetPubkey::Call(std::map<string,string> data){
  // Parameter
  int index            = atoi(GetValue(data, "index").c_str());
  // Get curve
  auto curve = uECC_secp256k1();
  // Get private key
  auto privkey = Storage::GetBlock(index);
  // Derive pubkey
  int pubkeysize = uECC_curve_public_key_size(curve);
  vector<char> pubkey(pubkeysize, 0);
  if(!uECC_compute_public_key((const uint8_t*)&privkey[0], (uint8_t*)&pubkey[0], curve)){
    throw "failed to compute public key";
  }
  // return
  std::map<string,string> resp;
  resp["curve"] = "secp256k1";
  resp["pubkey"] = BytesToHex(pubkey);
  return resp;
}

string Verify::Help(){ return "Verify signature (string curve, hex pubkey, hex msghash, hex signature)";}
std::map<string,string> Verify::Call(std::map<string,string> data){
  // Get params
  string scurve          = GetValue(data, "curve");
  vector<char> pubkey    = HexToBytes(GetValue(data, "pubkey"));
  vector<char> msghash   = HexToBytes(GetValue(data, "msghash"));
  vector<char> signature = HexToBytes(GetValue(data, "signature"));
  std::map<string,string> resp;
  // Select curve
  uECC_Curve curve;
  #define IF_CURVE(name) if(scurve==#name){ curve = uECC_##name(); } else
  IF_CURVE(secp160r1)
  IF_CURVE(secp192r1)
  IF_CURVE(secp224r1)
  IF_CURVE(secp256r1)
  IF_CURVE(secp256k1)
  {
    throw "curve unsupported";
  }
  // Verify
  if(!uECC_verify((const uint8_t*)&pubkey[0], (const uint8_t*)&msghash[0], msghash.size(), (const uint8_t*)&signature[0], curve)){
    throw "invalid signature";
  }
  resp["result"] = "true";
  return resp;
}
