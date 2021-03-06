#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <vector>
#include <map>

using namespace std;

class Protocol{
public:
  Protocol(void);

  string GetLine(void);
  void WriteLine(string);
  
  std::map<string,string> GetData(void);
  void WriteData(std::map<string,string>);

  bool available(void);
};

#endif
