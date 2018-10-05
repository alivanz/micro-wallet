#include "command.hpp"
#include "common.hpp"

class SetPrivateKey: virtual public Callable{
  string Help(){ return "WARNING: maybe unsafe! Set private key (string curve, hex privkey)";}
  std::map<string,string> Call(std::map<string,string> data){
    string curve           = GetValue(data, "curve");
    vector<char> pubkey    = HexToBytes(GetValue(data, "privkey"));
    std::map<string,string> resp;
    return resp;
  }
};

class CalcPubkey: virtual public Callable{
  string Help(){ return "Calculate public key (string curve, hex pubkey, hex msghash, hex signature)";}
  std::map<string,string> Call(std::map<string,string> data){
    string curve           = GetValue(data, "curve");
    vector<char> pubkey    = HexToBytes(GetValue(data, "pubkey"));
    vector<char> msghash   = HexToBytes(GetValue(data, "msghash"));
    vector<char> signature = HexToBytes(GetValue(data, "signature"));
    std::map<string,string> resp;
    resp["curve"] = curve;
    resp["pubkey"] = BytesToHex(pubkey);
    resp["msghash"] = BytesToHex(msghash);
    resp["signature"] = BytesToHex(signature);
    return resp;
  }
};

class Verify: virtual public Callable{
  string Help(){ return "Verify signature (string curve, hex pubkey, hex msghash, hex signature)";}
  std::map<string,string> Call(std::map<string,string> data){
    string curve           = GetValue(data, "curve");
    vector<char> pubkey    = HexToBytes(GetValue(data, "pubkey"));
    vector<char> msghash   = HexToBytes(GetValue(data, "msghash"));
    vector<char> signature = HexToBytes(GetValue(data, "signature"));
    std::map<string,string> resp;
    resp["curve"] = curve;
    resp["pubkey"] = BytesToHex(pubkey);
    resp["msghash"] = BytesToHex(msghash);
    resp["signature"] = BytesToHex(signature);
    return resp;
  }
};
