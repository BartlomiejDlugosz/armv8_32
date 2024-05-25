#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

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

  

  writeCPUState(argc, argv, &cpu);

  return EXIT_SUCCESS;
}
