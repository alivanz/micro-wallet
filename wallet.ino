#include "storage.hpp"
#include "command.hpp"
#include "ecc.hpp"
#include "trng.hpp"

Command *cmd;

void setup() {
  TRNG::Init();
  Storage::Init();
  cmd = new Command();
  cmd->methods["verify"] = new Verify();
  cmd->methods["setprivkey"] = new SetPrivateKey();
  cmd->methods["getpubkey"] = new GetPubkey();
  cmd->methods["signdeterministic"] = new SignDeterministic();
  cmd->methods["genprivkey"] = new GeneratePrivateKey();
  Serial.begin(115200);
}

void loop() {
  cmd->loop();
}
