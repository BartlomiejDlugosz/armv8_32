#ifndef BRANCH_H
#define BRANCH_H

#include "includes.h"

union branch_register {
  struct {
    unsigned : 5;
    unsigned xn : 5;
    unsigned : 16;
  };
  unsigned bits : 26;
};

union branch_conditional {
  struct {
    unsigned cond : 4;
    unsigned : 1;
    signed simm19 : 19;
    unsigned : 2;
  };
  unsigned bits : 26;
};

union branch_instruction {
  struct {
    signed simm26 : 26;
    unsigned : 4;
    unsigned type : 1;
    unsigned reg : 1;
  };
  unsigned bits : 32;
};

void branchExecute(CPU *cpu, uint32_t current_instr);

#endif  // BRANCH_H
