#include "command.hpp"
#include "ecc.hpp"

Command cmd;

void setup() {
  cmd = Command();
  cmd.methods["verify"] = new Verify();
  cmd.loop();
}

void loop() {
  cmd.loop();
}
