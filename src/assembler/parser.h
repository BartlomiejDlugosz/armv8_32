#ifndef PARSER_H
#define PARSER_H

#include "dynamicString.h"

typedef struct {
  // Dynamic since we don't know size of label
  dynamicString label;
  // Will always be a max of 10 characters
  char opcode[10];
  // Max number of operands is 4, may include labels with unkown size
  dynamicString operands[4];
} instruction;

instruction parse(char *);

// PARSER_H
#endif