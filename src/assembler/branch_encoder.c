#include <stdint.h>
#include <string.h>

#include "branch_encoder.h"
#include "parser.h"
#include "../emulator/branch.h"

#define COND_EQ_ENCODE 0b0000
#define COND_NE_ENCODE 0b0001
#define COND_GE_ENCODE 0b1010
#define COND_LT_ENCODE 0b1011
#define COND_GT_ENCODE 0b1100
#define COND_LE_ENCODE 0b1101
#define COND_AL_ENCODE 0b1110
#define COND_EQUAL "eq"
#define COND_NOT_EQUAL "ne"
#define COND_GREATER_OR_EQUAL "ge"
#define COND_LESS_THAN "lt"
#define COND_GREATER_THAN "gt"
#define COND_LESS_OR_EQUAL "le"
#define COND_ALWAYS "al"
#define XN_SHIFT 5
#define BRANCH_REGISTER_PADDING_SHIFT 16
#define BRANCH_REGISTER_PADDING 0b1000011111
#define BITMASK_19BITS 0x7FFFF
#define SIMM19_SHIFT 5
#define BITMASK_26BITS 0x3FFFFFF
#define BRANCH_INSTRUCTION_PADDING 0b0101
#define SIMM26_SHIFT 26
#define INSTR_TYPE_SHIFT 30
#define INSTR_REG_SHIFT 31

struct conditional_encoding {
    char cond[3];
    unsigned encoding : 4;
};

struct conditional_encoding conditional_encodings[] = {
    {.cond = COND_EQUAL, .encoding = COND_EQ_ENCODE},
    {.cond = COND_NOT_EQUAL, .encoding = COND_NE_ENCODE},
    {.cond = COND_GREATER_OR_EQUAL, .encoding = COND_GE_ENCODE},
    {.cond = COND_LESS_THAN, .encoding = COND_LT_ENCODE},
    {.cond = COND_GREATER_THAN, .encoding = COND_GT_ENCODE},
    {.cond = COND_LESS_OR_EQUAL, .encoding = COND_LE_ENCODE},
    {.cond = COND_ALWAYS, .encoding = COND_AL_ENCODE}};

static uint32_t generate_reg_instr(branch_register instr) {
    uint32_t bin = ((uint32_t)instr.xn) << XN_SHIFT;
    bin |= ((uint32_t)BRANCH_REGISTER_PADDING) << BRANCH_REGISTER_PADDING_SHIFT;
    return bin;
}

static uint32_t generate_cond_instr(branch_conditional instr) {
    uint32_t bin = 0;
    bin |= instr.cond;
    bin |= ((uint32_t)instr.simm19 & BITMASK_19BITS) << SIMM19_SHIFT;
    return bin;
}

static uint32_t generate_final_instr(branch_instruction instr) {
    uint32_t bin = 0;
    // Sign extension stuff
    bin |= instr.simm26 & BITMASK_26BITS;
    bin |= ((uint32_t)BRANCH_INSTRUCTION_PADDING) << SIMM26_SHIFT;
    bin |= ((uint32_t)instr.type) << INSTR_TYPE_SHIFT;
    bin |= ((uint32_t)instr.reg) << INSTR_REG_SHIFT;
    return bin;
}

uint32_t encode_branch(instruction *instr) {
    if (strcmp(instr->opcode, "b") == 0) {
        // b
        uint64_t target_literal =
            strtol(getString(instr->operands[0]) + 2, NULL, 16);

        branch_instruction final_instr = (branch_instruction){
            .simm26 = target_literal - instr->line_number, .type = 0, .reg = 0};
        return generate_final_instr(final_instr);
    } else if (strcmp(instr->opcode, "br") == 0) {
        // br
        uint64_t reg_number =
            strtol(getString(instr->operands[0]) + 1, NULL, 10);
        branch_register instr_reg = (branch_register){.xn = reg_number};
        branch_instruction final_instr = (branch_instruction){
            .simm26 = generate_reg_instr(instr_reg), .type = 1, .reg = 1};
        return generate_final_instr(final_instr);
    } else {
        // b.cond
        uint64_t target_literal =
            strtol(getString(instr->operands[0]) + 2, NULL, 16);
        char *opc = instr->opcode;
        uint8_t enc;
        opc += 2;
        for (int i = 0; i < sizeof(conditional_encodings) /
                                sizeof(struct conditional_encoding);
             i++) {
            if (strcmp(conditional_encodings[i].cond, opc) == 0) {
                enc = conditional_encodings[i].encoding;
                break;
            }
        }

        branch_conditional instr_cond = (branch_conditional){
            .cond = enc, .simm19 = target_literal - instr->line_number};
        branch_instruction final_instr = (branch_instruction){
            .simm26 = generate_cond_instr(instr_cond), .type = 1, .reg = 0};
        return generate_final_instr(final_instr);
    }
}