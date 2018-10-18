#include "storage.hpp"
#include "command.hpp"
#include "ecc.hpp"

Command *cmd;

void setup() {
  Storage::Init();
  cmd = new Command();
  cmd->methods["verify"] = new Verify();
  cmd->methods["setprivkey"] = new SetPrivateKey();
  cmd->methods["getpubkey"] = new GetPubkey();
  Serial.begin(115200);
}

void loop() {
  cmd->loop();
}
