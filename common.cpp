#include "common.hpp"
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>
#include <cstdarg>
#include <stdio.h>

string GetValue(std::map<string,string> data, string key){
  auto it = data.find(key);
  if(it == data.cend()){
    throw "key `"+key+"` not found";
  }
  return it->second;
}

vector<char> HexToBytes(string hex){;
  vector<char> bytes;
  for (unsigned int i = 0; i < hex.size(); i += 2) {
    std::string byteString = hex.substr(i, 2);
    char byte = (char) strtol(byteString.c_str(), NULL, 16);
    bytes.push_back(byte);
  }
  return bytes;
}

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
string BytesToHex(vector<char> bytes){
  string out(bytes.size()*2,' ');
  for (int i = 0; i < bytes.size(); ++i) {
    out[2 * i]     = hexmap[(bytes[i] & 0xF0) >> 4];
    out[2 * i + 1] = hexmap[bytes[i] & 0x0F];
  }
  return out;
}

string ssprintf(const char *format, ...){
  va_list argptr;
  va_start(argptr, format);
  char buffer[1024];
  int n = vsnprintf(buffer, sizeof(buffer), format, argptr);
  va_end(argptr);
  return string(buffer,n);
}

