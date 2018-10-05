#include <iostream>
#include <string>
#include <command.hpp>
#include <map>

int main(){
  Command cmd = Command();
  while(1) cmd.loop();
  return 0;
}
