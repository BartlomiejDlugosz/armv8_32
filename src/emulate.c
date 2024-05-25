#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void readBinaryToMemory(int argc, char **argv, CPU *cpu) {
  FILE *input_file;

  if (argc >= 1) {
    input_file = fopen(argv[0], "rb");
    if (input_file == NULL) {
      exit(1);
    }
  } else {
    exit(1);
  }

  fread(cpu->memory, MEMORY_SIZE, 1, input_file);

  fclose(input_file);
}

int main(int argc, char **argv) {
  CPU cpu;
  readBinaryToMemory(argc, argv, &cpu);
  extern uint32_t current_instr;
  extern branchExecute(int);
  extern dataProcessing(int, bool);
  extern loadStore (int);
  current_instr = read_memory(cpu, cpu->PC)
  while (current_instr != 0x8a000000) {
    current_instr = read_memory(cpu, cpu->PC)
    switch(current_instr & 0x1e000000) {
      case 0x10000000:
        // fall thru immediate DP
      case 0x12000000:
        dataProcessing(current_instr, true);
        break;
      case 0xA000000:
        // fall thru register Dp
      case 0x1A000000:
        dataProcessing(current_instr, false);
        break;
      case 0x8000000:
        // fall thru loadstore
      case 0x18000000:
      case 0x1C000000:
        loadStore(current_instr);
        break;
      case 0x14000000:
        // fall thru to branch execute
      case 0x16000000:
        branchExecute(current_instr);
        break;
    }
    cpu->PC += 4;
  }

  return EXIT_SUCCESS;
}
