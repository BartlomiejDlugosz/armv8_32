#include "single_data_transfer.h"
#include <stdint.h>

 void single_data_transfer(CPU *cpu, uint64_t *target_address, union single_data_transfer_instruction instr, union single_data_transfer_data data) {
    uint64_t xn_value = read_register64(cpu, data.xn);
    union single_data_transfer_data_offset offset = (union single_data_transfer_data_offset) {.bits = data.offset};

    if (instr.U == 1) {
        // Unsigned Immediate Offset
        // If sf == 1 (64 bit) then multiply by 8
        // else multiply by 4
        uint64_t uoffset = instr.sf ? data.offset * 8 : data.offset * 4;
        *target_address = xn_value + uoffset;
    } else if (offset.type == 0) {
        // Pre / Post Indexed
        if (offset.I == 1) {
            // Pre-Indexed
            *target_address = xn_value + offset.simm9;
            write_register64(cpu, data.xn, *target_address);
        } else {
            // Post Indexed
            *target_address = xn_value;
            write_register64(cpu, data.xn, *target_address + offset.simm9);
        }
    } else{
        // Register Offset
        // Manually extract bits to avoid to many nested union declarations
        uint64_t xm = (offset.bits >> 6) & 0x1F;
        uint64_t xm_value = read_register64(cpu, xm);
        *target_address = xn_value + xm_value;
    }
}

void single_data_transfer_init(CPU *cpu, uint32_t instruction) {
    // Initialize instr to the instruction
    union single_data_transfer_instruction instr = (union single_data_transfer_instruction) {.bits = instruction};
    union single_data_transfer_data data = (union single_data_transfer_data) {.bits = instr.data};

    uint64_t target_address;

    if (instr.type == 1) {
        // Single Data Transfer
        single_data_transfer(cpu, &target_address, instr, data);

    } else {
        // Load Literal
        uint64_t PC_value = cpu->PC;
        int64_t offset = instr.data * 4;
        target_address = PC_value + offset;
    }

    if (instr.type == 0 || data.L == 1) {
        printf("%x %x\n", instr.rt, read_memory(cpu, target_address, 8));
        // Load if Load Literal, or load flag set
        if (instr.sf == 1) {
            // 64 bit
            write_register64(cpu, instr.rt, read_memory(cpu, target_address, 8));
        } else {
            // 32 bit
            write_register32(cpu, instr.rt, read_memory(cpu, target_address, 4));
        }
    } else {
        // Chooses if we use 64 bit or 32 bit value
        uint64_t register_value = instr.sf ? read_register64(cpu, instr.rt) : read_register32(cpu, instr.rt);
        write_memory(cpu, target_address, register_value, instr.sf ? 8 : 4);
    }
}
