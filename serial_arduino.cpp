#include <Arduino.h>
#include <string>
#include <map>
#include "serial.hpp"

using namespace std;

Protocol::Protocol(void){
  Serial.begin(115200);
}

std::map<string,string> Protocol::GetData(void){
  std::map<string,string> out;
  for(string line; Serial.readStringUntil('\n'); ){
    if(line.length()==0){
      break;
    }
    size_t pos = line.find(" ");
    if(pos == string::npos){
      throw "invalid format";
    }
    out[line.substr(0,pos)] = line.substr(pos+1, string::npos);
  }
  return out;
}

void Protocol::WriteData(std::map<string,string> data){
  for(auto it=data.cbegin(); it!=data.cend(); it++){
    Serial.printf("%s", it->first);
    Serial.printf(" ");
    Serial.printf("%s\n", it->second);
  }
  Serial.println();
}

bool Protocol::available(void){
  return Serial.available();
}
