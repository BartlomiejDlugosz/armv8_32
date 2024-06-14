#include "single_data_transfer_assembly.h"
#include "../emulator/single_data_transfer.h"
#include "register_to_number.h"
#include <stdio.h>
#include <string.h>

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


static uint32_t generate_data_offset_binary(single_data_transfer_data_offset data_offset) {
    uint32_t bin = ((uint32_t)data_offset.tenth) << DATA_OFFSET_TENTH_BIT_SHIFT;
    bin |= ((uint32_t)data_offset.I) << DATA_OFFSET_I_BIT_SHIFT;
    bin |= ((uint32_t)data_offset.simm9 & DATA_OFFSET_SIMM9_MASK) << DATA_OFFSET_SIMM9_SHIFT;
    bin |= ((uint32_t)data_offset.type) << DATA_OFFSET_TYPE_BIT_SHIFT;
    return bin;
}

static uint32_t generate_data_binary(single_data_transfer_data data) {
    uint32_t bin = ((uint32_t)data.xn) << DATA_XN_SHIFT;
    bin |= ((uint32_t)data.offset) << DATA_OFFSET_SHIFT;
    bin |= ((uint32_t)data.L) << DATA_L_BIT_SHIFT;
    return bin;
}

static uint32_t generate_instruction_binary(single_data_transfer_instruction instr) {
    uint32_t bin = (uint32_t)instr.rt;
    bin |= ((uint32_t)instr.simm19 & INSTRUCTION_SIMM19_MASK) << INSTRUCTION_SIMM19_SHIFT;
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
        data_struct.L = 1; //Set L for load
    } else if (strcmp(instr->opcode, "str") == 0) {
        data_struct.L = 0; //Clear L for store
    }

    // Not dealing with stack pointer case
    // MACRO for below
    instr_struct.rt = register_to_number(getString(instr->operands[0]));
    char rt_type =  getString(instr->operands[0])[0];
    instr_struct.sf = (rt_type == 'x') ? 1 : 0;
    dynamicString* address_mode_array = createNewDynamicString(10);
    for (int i = 1; i < 5; i++) {
        addString(address_mode_array, getString(instr->operands[i]));
        if (getString(instr->operands[i]) != NULL) {
            addString(address_mode_array, ", ");
        }
    }
    // remove last two characters
    char* address_mode = getString(address_mode_array);
    address_mode[address_mode_array->current_size - 2] = '\0';

    if (address_mode[0] != '[') {
        // Load from literal
        // Two cases #N and an address
        instr_struct.opcode = 0b01100;
        int literal_int;
        int literal_address;
        data_struct.L = 0;
        if (sscanf(address_mode, "0x%x", &literal_address) == 1) {
            instr_struct.simm19 = literal_address  - instr->line_number;
        } else  if (sscanf(address_mode, "#0x%x", &literal_int) == 1) {
            instr_struct.simm19 = literal_int;
        } else if (sscanf(address_mode, "#%d", &literal_int) == 1) {
            instr_struct.simm19 = literal_int;
        }
    } else {
        instr_struct.type = 1; //Single Data Transfer
        instr_struct.opcode = 0b11100;
        int reg_num, offset, reg_m_num;
        //Handle the form [xn, #<imm>]
        // [x1''#0x8]'
        // '[x1, #32]'
        // Unsigned Immediate Offset
        char* cur_operand =  getString( instr->operands[2]);
        if (sscanf(address_mode, "[x%d, #%x]", &reg_num, &offset) == 2 && cur_operand[strlen(cur_operand)-1] != '!') {
            if (cur_operand[2] != 'x') {
                sscanf(address_mode, "[x%d, #%d]", &reg_num, &offset);
            }
            data_struct.xn = reg_num; // Register number assigned
            data_struct.offset = (offset / ((rt_type == 'x') ? 8 : 4)); //- instr->line_number; //Division dependent on rt_type
            instr_struct.U = 1;// Unsigned bit is set
        }
        // Handle the form [xn, #<simm>]!
        // Pre-Indexed
        //8b5a40b9
        // [x0, #0x120]'
        // split this into two cases (one for hex and one for dec)
        // special case with ! at the end (clean this later)
        else if(sscanf(address_mode, "[x%d, #%x]!", &reg_num, &offset) == 2) {
            if (cur_operand[2] != 'x') {
                sscanf(address_mode, "[x%d, #%d]!", &reg_num, &offset);
            }
            data_struct.xn = reg_num; // Register number assigned
            offset_struct.simm9 = offset; // Direct offset assignment
            offset_struct.I = 1; // Set I
            offset_struct.tenth = 1; // Set the tenth bit

        }
        // Handle the form [xn], #<simm>
        // Post-Indexed
        else if(sscanf(address_mode, "[x%d], #%d", &reg_num, &offset) == 2) {
            data_struct.xn = reg_num;
            offset_struct.simm9 = offset;
            offset_struct.tenth = 1; // Set the tenth bit
        }
        // Handle the form [xn, xm]
        // Register Offset
        else if(sscanf(address_mode, "[x%d, x%d]", &reg_num, &reg_m_num) == 2) {
            data_struct.xn = reg_num;
            offset_struct.simm9 = ((uint32_t)reg_m_num << 4) | 0b0110; // Shift 4 and mask
            offset_struct.type = 1;
            offset_struct.I = 1; // Set I
        } 
        //Handle the form [xn]
        // Zero Unsigned Offset
        // given [x5, x15]
        else if (sscanf(address_mode, "[x%d]", &reg_num) == 1) {
            data_struct.xn = reg_num; // Register number assigned
            instr_struct.U = 1;// Unsigned bit is se
        }
        else {
            fprintf(stderr, "Invalid single data transfer form\n");
        }
    }

    uint32_t offset =  generate_data_offset_binary(offset_struct);
    uint32_t data = generate_data_binary(data_struct);
    uint32_t instr_bin = generate_instruction_binary(instr_struct);
    binary_instr = offset | data | instr_bin;
    freeDynamicString(address_mode_array);
    return binary_instr;

}
