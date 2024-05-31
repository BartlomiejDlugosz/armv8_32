#include "includes.h"

// Reads the binary file with the given file name
void readBinaryToMemory(int argc, char **argv, CPU *cpu) {
  FILE *input_file;

  // Ensures that there are enough arguments provided
  if (argc > 1) {
    input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
      exit(1);
    }
  } else {
    exit(1);
  }

  // Reads the entire file into memory
  fread(cpu->memory, MEMORY_SIZE, 1, input_file);

  fclose(input_file);
}

void writeCPUState(int argc, char **argv, CPU *cpu) {
    if (argc > 2) {
        // A output file was given as a command line argument
        FILE *output_file;

        output_file = freopen(argv[2], "w", stdout);

        print_cpu_state(cpu);

        fclose(output_file);
    } else {
        print_cpu_state(cpu);
    }
}

int main(int argc, char **argv) {
  CPU cpu;
  initialise_cpu(&cpu);
  readBinaryToMemory(argc, argv, &cpu);
  uint32_t current_instr = read_memory(&cpu, cpu.PC, 4);
  while (current_instr != 0x8a000000) {
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
    cpu.PC += 4;
    current_instr = read_memory(&cpu, cpu.PC, 4);
  }
  writeCPUState(argc, argv, &cpu);
  return EXIT_SUCCESS;
}
