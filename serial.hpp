#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <vector>
#include <map>

using namespace std;

class Stream{
public:
  Stream(void);
  map<string,string> GetData(void);
  void WriteData(map<string,string>);

  bool available(void);
};

#endif
