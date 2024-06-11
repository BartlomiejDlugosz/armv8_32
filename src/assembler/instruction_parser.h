#ifndef INSTRCUTION_PARSER_H
#define INSTRUCTION_PARSER_H

#include "dynamicArray.h"
#include "symbol_table.h"

typedef struct {
    dynamicArray label;
    char opcode[10];
    dynamicArray operands[4];
} instruction;

instruction parse(const char* line);
uint32_t encode_instruction(instruction* instr, SymbolTable* table)
#endif // INSTRUCTION_PARSER_H