#include "single_data_transfer.h"
#include "structures.h"
#include <stdint.h>
#include <stdio.h>

 void single_data_transfer(CPU *cpu, uint64_t *target_address, union single_data_transfer_instruction instr) {
    uint64_t xn_value = read_register64(cpu, instr.data.xn);

    if (instr.U == 1) {
        // Unsigned Immediate Offset
        // If sf == 1 (64 bit) then multiply by 8
        // else multiply by 4
        uint64_t uoffset = instr.sf ? instr.data.offset * 8 : instr.data.offset * 4;
        *target_address = xn_value + uoffset;
    } else if (instr.data.type == 0) {
        // Pre / Post Indexed
        if (instr.data.I == 1) {
            // Pre-Indexed
            *target_address = xn_value + instr.data.simm9;
            write_register64(cpu, instr.data.xn, *target_address);
        } else {
            // Post Indexed
            *target_address = xn_value;
            write_register64(cpu, instr.data.xn, *target_address + instr.data.simm9);
        }
    } else{
        // Register Offset
        // Manually extract bits to avoid to many nested union declarations
        uint64_t xm = (instr.data.simm9 >> 6) & 0x1F;
        uint64_t xm_value = read_register64(cpu, xm);
        *target_address = xn_value + xm_value;
    }
}

void single_data_transfer_init(CPU *cpu, uint32_t instruction) {
    // Initialize instr to the instruction
    union single_data_transfer_instruction instr = (union single_data_transfer_instruction) {.bits = instruction};

    uint64_t target_address;

    if (instr.type == 1) {
        // Single Data Transfer
        single_data_transfer(cpu, &target_address, instr);

    } else {
        // Load Literal
        uint64_t PC_value = read_PC(cpu);
        int64_t offset = instr.data.bits * 4;
        target_address = PC_value + offset;
    }

    if (instr.type == 0 || instr.data.L == 1) {
        // Load if Load Literal, or load flag set
        if (instr.sf == 1) {
            // 64 bit
            write_register64(cpu, instr.rt, read_memory(cpu, target_address));
        } else {
            // 32 bit
            write_register32(cpu, instr.rt, read_memory(cpu, target_address));
        }
    } else {
        // Chooses if we use 64 bit or 32 bit value
        uint64_t register_value = instr.sf ? read_register64(cpu, instr.rt) : read_register32(cpu, instr.rt);
        write_memory(cpu, target_address, register_value);
    }
}