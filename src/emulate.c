#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char **argv) {
  extern int current_instr;
  extern int pc;
  extern branchExecute(int);
  extern dataProcessing(int, bool);
  extern loadStore (int);
  while (current_instr != 0x8a000000) {
    switch(current_instr & 0x1e000000) {
      case 0x10000000:
      case 0x12000000:
        dataProcessing(current_instr, true);
        break;
      case 0xA000000:
      case 0x1A000000:
        dataProcessing(current_instr, false);
        break;
      case 0x8000000:
      case 0x18000000:
      case 0x1C000000:
        loadStore(current_instr);
        break;
      case 0x14000000:
        // fall thru to op0 = 1011
      case 0x16000000:
        branchExecute(current_instr);
        break;
    }
    pc += 4;
  }
  return EXIT_SUCCESS;
}
