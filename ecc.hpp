#include "command.hpp"
#include <map>
#include <string>
using namespace std;

class SetPrivateKey: virtual public Callable{};
class GeneratePrivateKey: virtual public Callable{};
class GetPubkey: virtual public Callable{};
class CalcShared: virtual public Callable{};
class Sign: virtual public Callable{};
class SignDeterministic: virtual public Callable{};
class Verify: virtual public Callable{
  string Help();
  std::map<string,string> Call(std::map<string,string> data);
};
