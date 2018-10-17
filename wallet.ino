#include "command.hpp"
#include "ecc.hpp"

Command *cmd;

void setup() {
  cmd = new Command();
  cmd->methods["verify"] = new Verify();
  Serial.begin(115200);
}

void loop() {
  cmd->loop();
}
