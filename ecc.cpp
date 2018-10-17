#include "ecc.hpp"
#include "command.hpp"
#include "common.hpp"

#include <uECC.h>

using namespace std;

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
