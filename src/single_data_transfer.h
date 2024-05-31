#include <stdint.h>
#include "structures.h"
#pragma once

union single_data_transfer_data_offset {
    struct {
        unsigned : 1;
        unsigned I : 1;
        signed simm9 : 9;
        unsigned type : 1;
    };
    unsigned bits : 12;
};

// Defines the inner structure for the instruction which is dependent on whether
// it's a Single Data Transfer or Load Literal instruction
union single_data_transfer_data {
    struct {
        unsigned xn : 5;
        unsigned offset : 12;
        unsigned L : 1;
    };
    unsigned bits : 19;
};

// Defines union structure for the Single Data Transfer instructions
union single_data_transfer_instruction {
    struct {
        unsigned rt: 5;
        signed data : 19;
        unsigned U: 1;
        unsigned : 5;
        unsigned sf: 1;
        unsigned type: 1;
    };
    uint32_t bits;
};


void single_data_transfer(CPU *cpu, uint64_t *target_address, union single_data_transfer_instruction instr, union single_data_transfer_data data);

void single_data_transfer_init(CPU *cpu, uint32_t instruction);
