// Assuming symbol table is defined as symbol_table
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include "dynamicString.h"
#include "parser.h"

instruction parse(char* current_line, int *current_line) {
  instruction new_instruction;
  //Regex expression for determining if label
  regex_t rx;
  int regex_compiled;

  regex_compiled = regcomp(&rx, "[A-Za-z].*:", 0);
  if (regex_compiled != 0) {
      fprintf(stderr, "Failed to compile REGEX");
      exit(1);
  }
  
  // Initialise new instruction
  new_instruction.label = createNewDynamicString(10);
  memset(new_instruction.opcode, 0, sizeof(new_instruction.opcode));
  memset(new_instruction.operands, 0, sizeof(dynamicString) * 4);

  int match = regexec(&rx, current_line, 0, NULL, 0);
  if (match == 0) {
      // Matches REGEX for a label

  } else if (match == REG_NOMATCH) {
      // Instruction or empty line

  } else {
      // Error occured???

  }

  return new_instruction;
}

int main() {
    regex_t rx;
    int regex_compiled;

    regex_compiled = regcomp(&rx, "[A-Za-z].*:", 0);
    if (regex_compiled != 0) {
        printf("ERROR");
        exit(1);
    }
    regex_compiled = regexec(&rx, "    label:", 0, NULL, 0);
    printf("%i\n", regex_compiled);

    return 0;
}
