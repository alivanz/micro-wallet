#include <iostream>
#include <string>
#include <map>
#include <serial.hpp>

using namespace std;

Stream::Stream(void){}

map<string,string> Stream::GetData(void){
  map<string,string> out;
  for(string line; getline(cin, line); ){
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

void Stream::WriteData(map<string,string> data){
  for(auto it=data.cbegin(); it!=data.cend(); it++){
    cout << it->first << " " << it->second << endl;
  }
  cout << endl;
}

bool Stream::available(void){
  if(cin.eof()){
    exit(0);
  }
  return true;
}
