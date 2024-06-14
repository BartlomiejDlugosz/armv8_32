#ifndef CORE_INSTRUCTION_H
#define CORE_INSTRUCTION_H

#include <stdint.h>
#include "parser.h"

typedef struct {    
    const char *opcode;
    const char *operand0;
    const char *operand1;
    const char *operand2;
    const char *operand3;
    const char *operand4;
    const uint64_t line_number;
} core_instruction;

core_instruction *generate_core_instruction(instruction *instr);

#endif // CORE_INSTRUCTION_H
