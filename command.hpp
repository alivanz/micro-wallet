#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "serial.hpp"
#include <string>
#include <map>

class Callable{
public:
  virtual string Help() = 0;
  virtual std::map<string,string> Call(std::map<string,string>) = 0;
};

class Command{
  Protocol protocol;
public:
  Command();
  std::map<string, Callable*> methods;
  void loop();
  std::map<string,string> Help(std::map<string,string>);
};

#endif
