#include "command.hpp"
#include "serial.hpp"

using namespace std;

class HelpCmd: virtual public Callable{
  Command *cmd;
public:
  HelpCmd(Command *cmd){
    this->cmd = cmd;
  }
  string Help(){ return "this help";}
  map<string,string> Call(map<string,string> data){
    map<string,string> resp;
    for(auto it=cmd->methods.cbegin(); it!=cmd->methods.cend(); it++){
      resp[it->first] = it->second->Help();
    }
    return resp;
  }
};

class Echo: virtual public Callable{
  string Help(){ return "echo";}
  map<string,string> Call(map<string,string> data){
    return data;
  }
};

Command::Command(){
  protocol = Protocol();
  methods["help"] = new HelpCmd(this);
  methods["echo"] = new Echo();
}

void Command::loop(){
  if(protocol.available()){
    map<string,string> resp;
    auto request = protocol.GetData();
    try{
      auto method_it = request.find("method");
      if(method_it == request.cend()){
        throw "method not set";
      }
      auto callable_it = methods.find(method_it->second);
      if(callable_it == methods.cend()){
        throw "unknown method";
      }
      resp = callable_it->second->Call(request);
    } catch(const char* msg) {
      resp["error"] = msg;
    } catch(string msg) {
      resp["error"] = msg;
    }
    protocol.WriteData(resp);
  }
}
