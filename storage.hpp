#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <vector>

#define WalletLength 32
#define NWallet 16
#define NHeader NWallet
#define EEPROM_SIZE (NHeader + NWallet*WalletLength)

#define BlockAddr(block) (NHeader + block*WalletLength)

namespace Storage{
  void Init();
  int NBlock();
  char GetHeader(int);
  void SetBlock(int, std::vector<char>);
  std::vector<char> GetBlock(int);
}

#endif
