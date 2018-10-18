#include <EEPROM.h>
#include "storage.hpp"

namespace Storage{

void Init(){
  if (!EEPROM.begin(EEPROM_SIZE)){
    throw "Unable initialize EEPROM";
  }
}

int NBlock(){
  return NWallet;
}
char GetHeader(int i){
  return EEPROM.read(i);
}
void SetBlock(int i, std::vector<char> block){
  int addr = BlockAddr(i);
  for(int j=0; j<WalletLength; j++){
    EEPROM.write(addr+j, block[j]);
  }
  EEPROM.commit();
}
std::vector<char> GetBlock(int i){
  std::vector<char> out(WalletLength, 0);
  int addr = BlockAddr(i);
  for(int j=0; j<WalletLength; j++){
    out[j] = EEPROM.read(addr+j);
  }
  return out;
}

}
