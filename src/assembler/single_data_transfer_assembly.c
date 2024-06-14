#include <stdio.h>
#include <string.h>

#include "single_data_transfer_assembly.h"
#include "../emulator/single_data_transfer.h"
#include "register_to_number.h"

#define DATA_OFFSET_TENTH_BIT_SHIFT 10
#define DATA_OFFSET_I_BIT_SHIFT 11
#define DATA_OFFSET_SIMM9_MASK 0x1FF
#define DATA_OFFSET_SIMM9_SHIFT 12
#define DATA_OFFSET_TYPE_BIT_SHIFT 21

#define DATA_XN_SHIFT 5
#define DATA_OFFSET_SHIFT 10
#define DATA_L_BIT_SHIFT 22

#define INSTRUCTION_SIMM19_MASK 0x7FFFF
#define INSTRUCTION_SIMM19_SHIFT 5
#define INSTRUCTION_U_BIT_SHIFT 24
#define INSTRUCTION_OPCODE_SHIFT 25
#define INSTRUCTION_SF_BIT_SHIFT 30
#define INSTRUCTION_TYPE_BIT_SHIFT 31

#define BIT_DIVIDER_64 8
#define BIT_DIVIDER_32 4

#define SIMM9_OFFSET_SHIFT 4
#define SIMM9_OFFSET_MASK 0b0110

#define SINGLE_DATA_TRANSFER_OPCODE 0b11100
#define LITERAL_OPCODE 0b01100

static uint32_t generate_data_offset_binary(
    single_data_transfer_data_offset data_offset) {
    uint32_t bin = ((uint32_t)data_offset.tenth) << DATA_OFFSET_TENTH_BIT_SHIFT;
    bin |= ((uint32_t)data_offset.I) << DATA_OFFSET_I_BIT_SHIFT;
    bin |= ((uint32_t)data_offset.simm9 & DATA_OFFSET_SIMM9_MASK)
           << DATA_OFFSET_SIMM9_SHIFT;
    bin |= ((uint32_t)data_offset.type) << DATA_OFFSET_TYPE_BIT_SHIFT;
    return bin;
}

static uint32_t generate_data_binary(single_data_transfer_data data) {
    uint32_t bin = ((uint32_t)data.xn) << DATA_XN_SHIFT;
    bin |= ((uint32_t)data.offset) << DATA_OFFSET_SHIFT;
    bin |= ((uint32_t)data.L) << DATA_L_BIT_SHIFT;
    return bin;
}

static uint32_t generate_instruction_binary(
    single_data_transfer_instruction instr) {
    uint32_t bin = (uint32_t)instr.rt;
    bin |= ((uint32_t)instr.simm19 & INSTRUCTION_SIMM19_MASK)
           << INSTRUCTION_SIMM19_SHIFT;
    bin |= ((uint32_t)instr.U) << INSTRUCTION_U_BIT_SHIFT;
    bin |= ((uint32_t)instr.opcode) << INSTRUCTION_OPCODE_SHIFT;
    bin |= ((uint32_t)instr.sf) << INSTRUCTION_SF_BIT_SHIFT;
    bin |= ((uint32_t)instr.type) << INSTRUCTION_TYPE_BIT_SHIFT;
    return bin;
}

uint32_t single_data_transfer_to_binary(instruction* instr) {
    uint32_t binary_instr = 0;

    // Initialising instruction and data structures
    // All members of the structure are set to zero using {0}
    single_data_transfer_instruction instr_struct = {0};
    single_data_transfer_data data_struct = {0};
    single_data_transfer_data_offset offset_struct = {0};

    // Determine the opcode and set L
    if (strcmp(instr->opcode, "ldr") == 0) {
        data_struct.L = 1;  // Set L for load
    } else if (strcmp(instr->opcode, "str") == 0) {
        data_struct.L = 0;  // Clear L for store
    }

    char* target_reg = getString(instr->operands[0]);     // Get target register
    char* xn_or_literal = getString(instr->operands[1]);  // Get xn/literal
    char* code_offset = getString(instr->operands[2]);    // Get offset

    instr_struct.rt = register_to_number(target_reg);
    char rt_type = target_reg[0];
    instr_struct.sf = (rt_type == 'x') ? 1 : 0;

    if (xn_or_literal[0] == '[') {
        // Single Data Transfer
        instr_struct.type = 1;
        instr_struct.opcode = SINGLE_DATA_TRANSFER_OPCODE;
        int reg_num, offset, reg_m_num;
        char last_char = code_offset[strlen(code_offset) - 1];
        if (code_offset[0] == '\0') {
            // Zero Unsigned Offset
            // Handle the form [xn]
            sscanf(xn_or_literal, "[x%d]", &reg_num);
            data_struct.xn = reg_num;  // Register number assigned
            instr_struct.U = 1;        // Unsigned bit is set
        } else if (code_offset[0] != '#') {
            // Register Offset
            // Handle the form [xn, xm]
            sscanf(xn_or_literal, "[x%d", &reg_num);
            sscanf(code_offset, "x%d]", &reg_m_num);
            data_struct.xn = reg_num;
            offset_struct.simm9 =
                ((uint32_t)reg_m_num << SIMM9_OFFSET_SHIFT) | SIMM9_OFFSET_MASK;
            offset_struct.type = 1;  // Set type of offset
            offset_struct.I = 1;     // Set I
        } else if (last_char == '!') {
            // Pre-Indexed
            // Handle the general form [xn, #<simm>]!
            sscanf(xn_or_literal, "[x%d", &reg_num);
            sscanf(code_offset, "#%d]!", &offset);
            sscanf(code_offset, "#0x%x]!", &offset);
            data_struct.xn = reg_num;      // Register number assigned
            offset_struct.simm9 = offset;  // PC offset
            offset_struct.I = 1;           // Set I
            offset_struct.tenth = 1;       // Set the tenth bit
        } else if (last_char == ']') {
            // Unsigned Immediate Offset
            // Handle the form [xn, #<imm>]
            sscanf(xn_or_literal, "[x%d", &reg_num);
            sscanf(code_offset, "#%d]", &offset);
            sscanf(code_offset, "#0x%x]", &offset);
            data_struct.xn = reg_num;  // Register number assigned
            data_struct.offset =
                (offset /
                 ((rt_type == 'x')
                      ? BIT_DIVIDER_64
                      : BIT_DIVIDER_32));  // Division dependent on rt_type
            instr_struct.U = 1;            // Unsigned bit is set
        } else {
            // Post-Indexed
            // Handle the form [xn], #<simm>
            sscanf(xn_or_literal, "[x%d", &reg_num);
            sscanf(code_offset, "#%d", &offset);
            data_struct.xn = reg_num;      // Register number assigned
            offset_struct.simm9 = offset;  // PC offset
            offset_struct.tenth = 1;       // Set the tenth bit
        }
    } else {
        // Load from literal
        // Two cases #N and an label (which has been converted to an address by
        // the passing)
        instr_struct.opcode = LITERAL_OPCODE;
        int literal_int;
        int literal_address;
        data_struct.L = 0;
        if (xn_or_literal[0] == '#') {
            sscanf(xn_or_literal, "#%d", &literal_int);
            instr_struct.simm19 = literal_int;
        } else {
            sscanf(xn_or_literal, "0x%x", &literal_address);
            instr_struct.simm19 = literal_address - instr->line_number;
        }
    }
    // Generate the binary for each part of the instruction
    uint32_t offset = generate_data_offset_binary(offset_struct);
    uint32_t data = generate_data_binary(data_struct);
    uint32_t instr_bin = generate_instruction_binary(instr_struct);
    // Generate final binary instruction using bitwise or
    binary_instr = offset | data | instr_bin;
    return binary_instr;
}
