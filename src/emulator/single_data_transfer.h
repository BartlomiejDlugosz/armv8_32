#ifndef SINGLE_DATA_TRANSFER_H
#define SINGLE_DATA_TRANSFER_H

#include "includes.h"

// Defines the inner structure for the offset
typedef struct {
    unsigned tenth: 1;
    unsigned I : 1;
    signed simm9 : 9;
    unsigned type : 1;
} single_data_transfer_data_offset;

// Defines the inner structure for the instruction which is dependent on whether
// it's a Single Data Transfer or Load Literal instruction
typedef struct {
    unsigned xn : 5;
    unsigned offset : 12;
    unsigned L : 1;
    unsigned debug : 1;
} single_data_transfer_data;

// Defines union structure for the Single Data Transfer instructions
typedef struct {
    unsigned rt : 5;
    // Needs to be signed in the case of being used as a offset
    signed simm19 : 19;
    unsigned U : 1;
    unsigned opcode: 5;
    unsigned sf : 1;
    unsigned type : 1;
} single_data_transfer_instruction;

void single_data_transfer(CPU *cpu, uint64_t *target_address,
                          single_data_transfer_instruction instr,
                          single_data_transfer_data data);

void single_data_transfer_init(CPU *cpu, uint32_t instruction);
single_data_transfer_data_offset create_single_data_transfer_data_offset(
    uint32_t bits);
single_data_transfer_data create_single_data_transfer_data(uint32_t bits);
single_data_transfer_instruction create_single_data_transfer_instruction(
    uint32_t bits);

#endif //SINGLE_DATA_TRANSFER_H