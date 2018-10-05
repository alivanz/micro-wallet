#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <serial.hpp>
#include <string>
#include <map>

class Callable{
public:
  virtual string Help() = 0;
  virtual map<string,string> Call(map<string,string>) = 0;
};

class Command{
  Stream stream;
public:
  Command();
  map<string, Callable*> methods;
  void loop();
  map<string,string> Help(map<string,string>);
};

#endif
