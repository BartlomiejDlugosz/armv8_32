#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

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

  return EXIT_SUCCESS;
}
