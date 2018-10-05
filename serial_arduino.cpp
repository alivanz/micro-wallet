#include <Arduino.h>
#include <string>
#include <map>
#include "serial.hpp"

using namespace std;

Protocol::Protocol(void){
  Serial.begin(115200);
}

string readline(){
  String line;
  while(1){
    try{
      line += Serial.readStringUntil('\n');
      break;
    }catch(...){
    }
  }
  line.trim();
  unsigned char s[1024];
  line.getBytes(s,sizeof(s));
  return string((const char*)s, line.length());
}
String sconv(string s){
  return String(s.c_str());
}

std::map<string,string> Protocol::GetData(void){
  std::map<string,string> out;
  for(string line;; ){
    line=readline();
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
    Serial.print(sconv(it->first));
    Serial.printf(" ");
    Serial.print(sconv(it->second));
    Serial.println();
  }
  Serial.println();
}

bool Protocol::available(void){
  return Serial.available();
}
