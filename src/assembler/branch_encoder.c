#include <stdint.h>
#include <string.h>

#include "branch_encoder.h"
#include "parser.h"
#include "../emulator/branch.h"

struct conditional_encoding {
    char cond[3];
    unsigned encoding : 4;
};

struct conditional_encoding conditional_encodings[] =
    {
        {.cond = "eq", .encoding=0b0000},
        {.cond = "ne", .encoding=0b0001},
        {.cond = "ge", .encoding=0b1010},
        {.cond = "lt", .encoding=0b1011},
        {.cond = "gt", .encoding=0b1100},
        {.cond = "le", .encoding=0b1101},
        {.cond = "al", .encoding=0b1110}
    };

static uint32_t generate_reg_instr(branch_register instr) {
    uint32_t bin = ((uint32_t) instr.xn) << 5;
    bin |= ((uint32_t) 0b1000011111) << 16;
    return bin;
}

static uint32_t generate_cond_instr(branch_conditional instr) {
    uint32_t bin = 0;
    bin |= instr.cond;
    bin |= ((uint32_t) instr.simm19 & 0x7FFFF) << 5;
    return bin;
}

static uint32_t generate_final_instr(branch_instruction instr) {
    uint32_t bin = 0;
    // Sign extension stuff
    bin |= instr.simm26 & 0x3FFFFFF;
    bin |= ((uint32_t)0b0101) << 26;
    bin |= ((uint32_t)instr.type) << 30;
    bin |= ((uint32_t) instr.reg) << 31;
    return bin;
}

uint32_t encode_branch(instruction *instr) {

    if (strcmp(instr->opcode, "b") == 0) {
        // b
        uint64_t target_literal = strtol(getString(instr->operands[0]) + 2, NULL, 16);

        branch_instruction final_instr =
            (branch_instruction) {.simm26 = target_literal - instr->line_number,
                                  .type = 0,
                                  .reg = 0};
        return generate_final_instr(final_instr);
    } else if (strcmp(instr->opcode, "br") == 0) {
        // br
        uint64_t reg_number = strtol(getString(instr->operands[0]) + 1, NULL, 10);
        branch_register instr_reg =
            (branch_register) {.xn = reg_number};
        branch_instruction final_instr =
            (branch_instruction) {.simm26 = generate_reg_instr(instr_reg),
                                  .type = 1,
                                  .reg = 1};
        return generate_final_instr(final_instr);
    } else {
        // b.cond
        uint64_t target_literal = strtol(getString(instr->operands[0]) + 2, NULL, 16);
        char *opc = instr->opcode;
        uint8_t enc;
        opc += 2;
        for (int i = 0; i < sizeof(conditional_encodings) / sizeof(struct conditional_encoding); i++) {
            if (strcmp(conditional_encodings[i].cond, opc) == 0) {
                enc = conditional_encodings[i].encoding;
                break;
            }
        }

        branch_conditional instr_cond =
            (branch_conditional) {.cond = enc, .simm19 = target_literal - instr->line_number};
        branch_instruction final_instr =
            (branch_instruction) {.simm26 = generate_cond_instr(instr_cond),
                                  .type = 1,
                                  .reg = 0};
        return generate_final_instr(final_instr);
    }
}