#include "parser.h"
#include "dynamicString.h"
#include "core_instruction.h"

core_instruction *generate_core_instruction(instruction *instr) {
    core_instruction *new_cinstr;

    new_cinstr->opcode = instr->opcode;
    new_cinstr->operand0 = getString(instr->operand[0]);
    new_cinstr->operand1 = getString(instr->operand[1]);
    new_cinstr->operand2 = getString(instr->operand[2]);
    new_cinstr->operand3 = getString(instr->operand[3]);
    new_cinstr->operand4 = getString(instr->operand[4]);
    new_cinstr->line_number = instr->line_number;

    return new_cinstr;
}
