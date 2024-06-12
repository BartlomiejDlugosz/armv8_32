#include "single_data_transfer_assembly.h"
#include "../emulator/single_data_transfer.h"
#include <stdio.h>
#include <string.h>

static uint32_t generate_data_offset_binary(single_data_transfer_data_offset data_offset) {
    uint32_t bin = (uint32_t)data_offset.tenth << 10;
    bin |= (uint32_t)data_offset.I << 11;
    bin |= ((uint32_t)data_offset.simm9 & 0x1FF) << 20;
    bin |= (uint32_t)data_offset.type << 21;
    return bin;
}

static uint32_t generate_data_binary(single_data_transfer_data data) {
    uint32_t bin = (uint32_t)data.xn << 9;
    bin |= (uint32_t)data.offset << 21;
    bin |= (uint32_t)data.L << 22;
    return bin;
}

static uint32_t generate_instruction_binary(single_data_transfer_instruction instr) {
    uint32_t bin = (uint32_t)instr.rt;
    bin |= ((uint32_t)instr.simm19 & 0x1F) << 23;
    bin |= (uint32_t)instr.U << 24;
    bin |= (uint32_t)instr.opcode << 29;
    bin |= (uint32_t)instr.sf << 30;
    bin |= (uint32_t)instr.type << 31; 
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
    // Have to deal with zero register
    // MACRO for below
    instr_struct.rt = atoi(&(getString(instr->operands[0])[1])); // x3
    char rt_type =  getString(instr->operands[0])[0];
    instr_struct.sf = (rt_type == 'x') ? 1 : 0;
    char* address_mode = getString(instr->operands[1]);

    if (address_mode[0] == '[') {
        instr_struct.type = 1; //Single Data Transfer
        instr_struct.opcode = 0b1110;
        int reg_num, offset, reg_m_num;
        //Handle the form [xn]
        // Zero Unsigned Offset
        if (sscanf(address_mode, "[x%d]", &reg_num) == 1) {
            data_struct.xn = reg_num; // Register number assigned
            instr_struct.U = 1; // Unsigned bit is set
        }
        //Handle the form [xn, #<imm>]
        // Unsigned Immediate Offset
        else if(sscanf(address_mode, "[x%d, #%d]", &reg_num, &offset) == 2) {
            data_struct.xn = reg_num; // Register number assigned
            data_struct.offset = offset / ((rt_type == 'x') ? 8 : 4); //Division dependent on rt_type
            instr_struct.U = 1;// Unsigned bit is set
        }
        // Handle the form [xn, #<simm>]!
        // Pre-Indexed
        else if(sscanf(address_mode, "[x%d, #%d]!", &reg_num, &offset) == 2) {
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
            offset_struct.simm9 = (reg_m_num << 4) | 0b110; // Shift 4 and mask
            offset_struct.type = 1;
            offset_struct.I = 1; // Set I
        } else {
            fprintf(stderr, "Invalid single data transfer form\n");
        }
    }
    else {
        // Load from literal
        // Two cases #N and an address
        instr_struct.opcode = 0b01100;
        int literal_int;
        int literal_address;
        if (sscanf(address_mode, "#%d", &literal_int) == 1) {
            instr_struct.simm19 = literal_int / 4;
        } else if (sscanf(address_mode, "%d", &literal_address) == 1) {
            instr_struct.simm19 = literal_int / 4;
        }
    }

    uint32_t offset =  generate_data_offset_binary(offset_struct);
    uint32_t data = generate_data_binary(data_struct);
    uint32_t instr_bin = generate_instruction_binary(instr_struct);
    binary_instr = offset | data | instr_bin;
    return binary_instr;

}
