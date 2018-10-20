#include "ecc.hpp"
#include "command.hpp"
#include "common.hpp"
#include "storage.hpp"
extern "C"{
  #include "sha2.h"
}

#include <uECC.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "trng.hpp"
#include <string.h>

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

string GetCurve::Help(){ return "Get curve (int index) => (curve)"; }
std::map<string,string> GetCurve::Call(std::map<string,string> data){
  std::map<string,string> resp;
  resp["curve"] = "secp256k1";
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

typedef struct SHA256_HashContext {
    uECC_HashContext uECC;
    SHA256_CTX ctx;
} SHA256_HashContext;

static void init_SHA256(const uECC_HashContext *base) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    sha256_Init(&context->ctx);
}

static void update_SHA256(const uECC_HashContext *base,
                          const uint8_t *message,
                          unsigned message_size) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    sha256_Update(&context->ctx, message, message_size);
}

static void finish_SHA256(const uECC_HashContext *base, uint8_t *hash_result) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    sha256_Final(hash_result, &context->ctx);
}

string SignDeterministic::Help(){ return "Deterministic Signature (int index, hex msghash) => (hex signature)"; };
std::map<string,string> SignDeterministic::Call(std::map<string,string> data){
  // Parameter
  int index            = atoi(GetValue(data, "index").c_str());
  vector<char> msghash   = HexToBytes(GetValue(data, "msghash"));
  // Get curve
  auto curve = uECC_secp256k1();
  // Get private key
  auto privkey = Storage::GetBlock(index);
  // Signature
  vector<char> signature(64,0);
  uint8_t tmp[2 * SHA256_DIGEST_LENGTH + SHA256_BLOCK_LENGTH];
    SHA256_HashContext ctx = {{
        &init_SHA256,
        &update_SHA256,
        &finish_SHA256,
        SHA256_BLOCK_LENGTH,
        SHA256_DIGEST_LENGTH,
        tmp
    }};
  if(!uECC_sign_deterministic((const uint8_t *)&privkey[0], (const uint8_t *)&msghash[0], msghash.size(), &ctx.uECC, (uint8_t *)&signature[0], curve)){
    throw "Signature failed";
  }
  // return
  std::map<string,string> resp;
  resp["signature"] = BytesToHex(signature);
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

int RNG(uint8_t *dest, unsigned size){
  uint32_t data;
  while(size>0){
    data = TRNG::Read();
    unsigned l = size;
    if(l>sizeof(data)){
      l = sizeof(data);
    }
    memcpy(dest, &data, l);
    size = size - l;
  }
  return 1;
}
GeneratePrivateKey::GeneratePrivateKey(){
  uECC_set_rng(RNG);
}
string GeneratePrivateKey::Help(){ return "Generate key from TRNG (int index, string curve)"; }
std::map<string,string> GeneratePrivateKey::Call(std::map<string,string> data){
  // Parameter
  int index            = atoi(GetValue(data, "index").c_str());
  string scurve        = GetValue(data, "curve");
  // Check Curve
  if(scurve!="secp256k1"){
    throw "unsupported curve";
  }
  uECC_Curve curve = uECC_secp256k1();
  // Generate key
  vector<char> privkey(64,0);
  vector<char> pubkey(64,0);
  if(!uECC_make_key((uint8_t*)&pubkey[0],(uint8_t*)&privkey[0],curve)){
    throw "Failed to create key";
  }
  // Set private key
  Storage::SetBlock(index, privkey);
  // Result
  std::map<string,string> resp;
  return resp;
}
