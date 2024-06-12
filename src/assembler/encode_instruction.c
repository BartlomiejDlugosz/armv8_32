#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"
#include "encode_instruction.h"

#define NUM_HALTING_OPERANDS 3

// all arrays are null terminated, for easy iteration when passing to functions
char *data_processing_opcodes[] = { "add", "adds", "sub", "subs", "cmp", "cmn", "neg", "negs", "and", "ands", "bic", "bics", "eor", "eon", "orr", "orn", "tst", "mvn", "mov", "movn", "movk", "movz", "madd", "msub", "mul", "mneg", NULL };
char *branch_opcodes[] = { "b", "br", "b.cond", NULL };
char *single_data_transfer_opcodes[] = { "ldr", "str", NULL };
char *directive_opcodes[] = { ".int", NULL };


// special case of is_type
/*
static bool is_halt(instruction *instr) { 
    if (strcmp(instr->opcode, "and") != 0) {
        return false;
    }

    for (int i = 0; i < NUM_HALTING_OPERANDS; i++) {
        if (strcmp(getString(instr->operands[0]), "x0") != 0) {
            return false;
        }
    }

    return true;
}
*/

static bool is_type(instruction *instr, char **opcodes) {
    for (int i = 0; opcodes[i] != NULL; i++) {
        if (strcmp(instr->opcode, opcodes[i]) == 0) {
            return true;
        }
    }

    return false;
}


uint32_t encode_instruction(instruction *instr) {
    // first identify if we're dealing with a halt instruction
    // in which case we immediately return the halt instruction
    // defined above. Note that the halt is in big endian,
    // but will be converted to little endian when writing to 
    // the binary file
    
    /*
    if (is_halt(instr)) {
        return HALT_INSTRUCTION_BIG_ENDIAN;
    }
    */

    if (is_type(instr, data_processing_opcodes)) {
        // call
    }

    if (is_type(instr, branch_opcodes)) {
        // call
    }

    if (is_type(instr, single_data_transfer_opcodes)) {
        // call
    }

    if (is_type(instr, directive_opcodes)) {
        // call
    }

    printf("The opcode is not defined!\n");
    exit(1);
}
