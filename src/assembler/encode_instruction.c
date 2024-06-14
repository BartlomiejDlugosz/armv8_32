#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"
#include "encode_instruction.h"
#include "branch_encoder.h"
#include "single_data_transfer_assembly.h"
#include "data_processing_assembly.h"

// all arrays are null terminated, for easy iteration when passing to functions
char *data_processing_opcodes[] = { "add", "adds", "sub", "subs", "cmp", "cmn", "neg", "negs", "and", "ands", "bic", "bics", "eor", "eon", "orr", "orn", "tst", "mvn", "mov", "movn", "movk", "movz", "madd", "msub", "mul", "mneg", NULL };
char *branch_opcodes[] = { "b", "br", "b.eq", "b.ne", "b.ge", "b.lt", "b.gt", "b.le", "b.al", NULL };
char *single_data_transfer_opcodes[] = { "ldr", "str", NULL };
char *directive_opcodes[] = { ".int", NULL };

static bool is_type(instruction *instr, char **opcodes) {
    for (int i = 0; opcodes[i] != NULL; i++) {
        if (strcmp(instr->opcode, opcodes[i]) == 0) {
            return true;
        }
    }

    return false;
}


uint32_t encode_directive(instruction *instr) {
    uint32_t encoded_n;
    char *n = getString(instr->operands[0]);

    // .int <123499 | 0xFA223>
    // check if hex
    if (sscanf(n, "0x%x", &encoded_n) == 1) {
        return encoded_n;
    }
    // check if denary
    if (sscanf(n, "%d", &encoded_n) == 1) {
        return encoded_n;
    }

    fprintf(stderr, "encode_instruction: Invalid operand '%s'to .int directive!\n", n);
    return 0;
}


uint32_t encode_instruction(instruction *instr) {
    if (is_type(instr, data_processing_opcodes)) {
        return data_processing_assembly_init(instr);
    }

    if (is_type(instr, branch_opcodes)) {
        return encode_branch(instr);
    }

    if (is_type(instr, single_data_transfer_opcodes)) {
        return single_data_transfer_to_binary(instr);
    }

    if (is_type(instr, directive_opcodes)) {
        return encode_directive(instr);
    }

    fprintf(stderr, "encode_instrction: The opcode is not defined!\n");
    return 0;
}
