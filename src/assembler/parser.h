#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include "dynamicString.h"

#define MAX_OPCODE_LENGTH 10
#define MAX_OPERANDS 5

typedef struct {
    // Dynamic since we don't know size of label
    dynamicString *label;
    // Will always be a max of 10 characters
    char opcode[MAX_OPCODE_LENGTH];
    // Max number of operands is 5, may include labels with unkown size
    dynamicString *operands[MAX_OPERANDS];
    // Boolean to let others know if instruction is complete, or still in
    // the process of being created
    bool complete;
    // Stores the line number of the current instruction
    uint64_t line_number;
} instruction;

void pretty_print(instruction *instr);
void free_instruction(instruction *, bool);
void initialize_instruction(instruction *, bool);
instruction *parse(char *, uint64_t *);

// PARSER_H
#endif
