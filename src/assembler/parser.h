#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "dynamicString.h"

typedef struct {
    // Dynamic since we don't know size of label
    dynamicString label;
    // Will always be a max of 10 characters
    char opcode[10];
    // Max number of operands is 4, may include labels with unkown size
    dynamicString operands[4];
    // Boolean to let others know if instruction is complete, or still in
    // the process of being created
    bool complete;
} instruction;

instruction *parse(char *, int *);

// PARSER_H
#endif