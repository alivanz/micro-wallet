#include "command.hpp"
#include <map>
#include <string>
using namespace std;

class SetPrivateKey: virtual public Callable{
  string Help();
  std::map<string,string> Call(std::map<string,string> data);
};
class GeneratePrivateKey: virtual public Callable{
  public:
  GeneratePrivateKey();
  string Help();
  std::map<string,string> Call(std::map<string,string> data);
};
class GetPubkey: virtual public Callable{
  string Help();
  std::map<string,string> Call(std::map<string,string> data);
};
class CalcShared: virtual public Callable{};
class Sign: virtual public Callable{};
class SignDeterministic: virtual public Callable{
  string Help();
  std::map<string,string> Call(std::map<string,string> data);
};
class Verify: virtual public Callable{
  string Help();
  std::map<string,string> Call(std::map<string,string> data);
};
