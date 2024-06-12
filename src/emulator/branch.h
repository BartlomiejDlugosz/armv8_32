#ifndef BRANCH_H
#define BRANCH_H

#include "includes.h"

typedef struct {
    unsigned : 5;
    unsigned xn : 5;
    unsigned : 16;
} branch_register;

typedef struct {
    unsigned cond : 4;
    unsigned : 1;
    signed simm19 : 19;
    unsigned : 2;
} branch_conditional;

typedef struct {
    signed simm26 : 26;
    unsigned : 4;
    unsigned type : 1;
    unsigned reg : 1;
} branch_instruction;

void branchExecute(CPU *cpu, uint32_t current_instr);
branch_register create_branch_register(uint32_t bits);
branch_conditional create_branch_conditional(uint32_t bits);
branch_instruction create_branch_instruction(uint32_t bits);

#endif // BRANCH_H
