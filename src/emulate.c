#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "structures.h"
#include "structures.c"

int main(int argc, char **argv) {
  uint32_t current_instr = read_memory(&cpu, cpu->PC, 4);
  while (current_instr != 0x8a000000) {
    current_instr = read_memory(&cpu, cpu->PC, 4);
    switch(current_instr & 0x1e000000) {
      case 0x10000000:
        // fall thru immediate DP
      case 0x12000000:
        data_processing_init(&cpu, current_instr, true);
        break;
      case 0xA000000:
        // fall thru register Dp
      case 0x1A000000:
        data_processing_init(&cpu, current_instr, false);
        break;
      case 0x8000000:
        // fall thru loadstore
      case 0x18000000:
      case 0x1C000000:
        single_data_transfer_init(&cpu, current_instr);
        break;
      case 0x14000000:
        // fall thru to branch execute
      case 0x16000000:
        branchExecute(&cpu, current_instr);
        break;
    }
    cpu->PC += 4;
  }
  
  return EXIT_SUCCESS;
}
