#include "includes.h"

#define EXTRACT_XM(bits) (bits >> 6) & 0b11111

#define STACK_POINTER_REGISTER 0b11111
#define SIMM19_MULTIPLIER 4
#define OFFSET_MULTIPLIER_64 8
#define OFFSET_MULTIPLIER_32 4
#define BYTES_64 8
#define BYTES_32 4

#define I_OFFSET_LOCATION 1
#define SIMM9_OFFSET_LOCATION 2
#define TYPE_OFFSET_LOCATION 11
#define OFFSET_DATA_LOCATION 5
#define L_DATA_LOCATION 17
#define SIMM19_INSTR_LOCATION 5
#define U_INSTR_LOCATION 24
#define SF_INSTR_LOCATION 30
#define TYPE_INSTR_LOCATION 31

single_data_transfer_data_offset create_single_data_transfer_data_offset(
    uint32_t bits) {
    single_data_transfer_data_offset instr;
    instr.I = (bits >> I_OFFSET_LOCATION);
    instr.simm9 = (bits >> SIMM9_OFFSET_LOCATION);
    instr.type = (bits >> TYPE_OFFSET_LOCATION);
    return instr;
}

single_data_transfer_data create_single_data_transfer_data(uint32_t bits) {
    single_data_transfer_data instr;
    instr.xn = bits;
    instr.offset = (bits >> OFFSET_DATA_LOCATION);
    instr.L = (bits >> L_DATA_LOCATION);
    return instr;
}

single_data_transfer_instruction create_single_data_transfer_instruction(
    uint32_t bits) {
    single_data_transfer_instruction instr;
    instr.rt = bits;
    instr.simm19 = (bits >> SIMM19_INSTR_LOCATION);
    instr.U = (bits >> U_INSTR_LOCATION);
    instr.sf = (bits >> SF_INSTR_LOCATION);
    instr.type = (bits >> TYPE_INSTR_LOCATION);
    return instr;
}

void single_data_transfer(CPU *cpu, uint64_t *target_address,
                          single_data_transfer_instruction instr,
                          single_data_transfer_data data) {
    // We are not handling the case where xn refers to the stack pointer
    assert(data.xn != STACK_POINTER_REGISTER);

    uint64_t xn_value = read_register64(cpu, data.xn);
    single_data_transfer_data_offset offset =
        create_single_data_transfer_data_offset(data.offset);

    if (instr.U == 1) {
        // Unsigned Immediate Offset
        // If sf == 1 (64 bit) then multiply by 8
        // else multiply by 4
        uint64_t uoffset = instr.sf ? data.offset * OFFSET_MULTIPLIER_64
                                    : data.offset * OFFSET_MULTIPLIER_32;
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
    } else {
        // Register Offset
        // Manually extract bits to avoid too many nested union declarations
        uint64_t xm = EXTRACT_XM(data.offset);
        uint64_t xm_value = read_register64(cpu, xm);
        *target_address = xn_value + xm_value;
    }
}

void single_data_transfer_init(CPU *cpu, uint32_t instruction) {
    // Initialize instr to the instruction
    single_data_transfer_instruction instr =
        create_single_data_transfer_instruction(instruction);
    single_data_transfer_data data =
        create_single_data_transfer_data(instr.simm19);

    // Define the final target_address which will either be read from, or
    // written to
    uint64_t target_address = 0;

    if (instr.type == 1) {
        // Single Data Transfer
        single_data_transfer(cpu, &target_address, instr, data);
    } else {
        // Load Literal
        uint64_t PC_value = cpu->PC;
        int64_t offset = instr.simm19 * SIMM19_MULTIPLIER;
        target_address = PC_value + offset;
    }

    // Load if Load Literal, or load flag set
    if (instr.type == 0 || data.L == 1) {
        if (instr.sf == 1) {
            // 64 bit
            write_register64(cpu, instr.rt,
                             read_memory(cpu, target_address, BYTES_64));
        } else {
            // 32 bit
            write_register32(cpu, instr.rt,
                             read_memory(cpu, target_address, BYTES_32));
        }
    } else {
        // Chooses if we use 64 bit or 32 bit value
        // In the case of 32 bits the upper 32 bits are ignored
        uint64_t register_value = instr.sf ? read_register64(cpu, instr.rt)
                                           : read_register32(cpu, instr.rt);
        write_memory(cpu, target_address, register_value,
                     instr.sf ? BYTES_64 : BYTES_32);
    }
}
