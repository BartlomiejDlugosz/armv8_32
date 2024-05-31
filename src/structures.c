#include "includes.h"

#define MAX_BYTES 8
#define BYTE_WIDTH 8
#define ADDRESS_ALIGNMENT 4
#define ZR_REG_INDEX 31

// Better name (maybe?) : reg_and_mem_functions.c
// Initialise the CPU state
void initialise_cpu(CPU *cpu) {
    memset(cpu, 0, sizeof(CPU));
    cpu->pstate.Z = 1; // INITIAL PSTATE value with Z flag set
}

// Read a 64-bit value from a general-purpose register
uint64_t read_register64(const CPU *cpu, uint8_t reg_index) {
    if (reg_index < NUM_REGISTERS) {
        return cpu->registers[reg_index];
    } else if (reg_index == ZR_REG_INDEX) {
        return 0; // ZR always returns 0
    } else {
        PRINT_INVALID_REGISTER_INDEX(reg_index);
    }
}

// Read a 32-bit value from a general-purpose register
uint32_t read_register32(const CPU* cpu, uint8_t reg_index) {
    if (reg_index < NUM_REGISTERS) {
        return (uint32_t)(cpu->registers[reg_index] & 0xFFFFFFFF); // Masks the lower 32 bits
    } else if (reg_index == ZR_REG_INDEX) {
        return 0; // ZR always returns 0
    } else {
        PRINT_INVALID_REGISTER_INDEX(reg_index);
    }
}

// Write a 64-bit value to a general-purpose register
void write_register64(CPU *cpu, uint8_t reg_index, uint64_t value) {
    if (reg_index < NUM_REGISTERS) {
        cpu->registers[reg_index] = value;
    } else if (reg_index != ZR_REG_INDEX) { // Writes to ZR are ignored
        PRINT_INVALID_REGISTER_INDEX(reg_index);
    }
}

// Write a 32-bit value to a general-purpose register
void write_register32(CPU *cpu, uint8_t reg_index, uint32_t value) {
    if (reg_index < NUM_REGISTERS) {
        cpu->registers[reg_index] = (uint64_t)value;
    } else if (reg_index != ZR_REG_INDEX) { // Writes to ZR are ignored
        PRINT_INVALID_REGISTER_INDEX(reg_index);
    }
}

// Read a value from memory in little-endian format
uint64_t read_memory(const CPU *cpu, uint32_t address, uint8_t num_bytes) {
    if (address + num_bytes - 1 < MEMORY_SIZE && num_bytes <= MAX_BYTES) {
        uint64_t value = 0;
        for (uint8_t i = 0; i < num_bytes; i++) {
            value |= ((uint64_t)cpu->memory[address + i]) << (BYTE_WIDTH * i);
        }
        return value;
    } else {
        PRINT_INVALID_MEMORY_ACCESS(address);
        return 0;
    }
}

// Write a value to memory in little-endian format
void write_memory(CPU *cpu, uint32_t address, uint64_t value, uint8_t num_bytes) {
    if (address + num_bytes - 1 < MEMORY_SIZE && num_bytes <= MAX_BYTES) {
        for (uint8_t i = 0; i < num_bytes; i++) {
            cpu->memory[address + i] = (value >> (BYTE_WIDTH * i)) & 0xFF;
        }
    } else {
        PRINT_INVALID_MEMORY_ACCESS(address);
    }
}

// Print the current state of the CPU
void print_cpu_state(const CPU *cpu) {
    // Print registers
    printf("Registers:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("X%02d = %016lx\n", i, cpu->registers[i]);
    }
    // Print program counter
    printf("PC = %016lx\n", cpu->PC);
    // Print PSTATE
    printf("PSTATE: %c%c%c%c\n",
            cpu->pstate.N ? 'N' : '-',
            cpu->pstate.Z ? 'Z' : '-',
            cpu->pstate.C ? 'C' : '-',
            cpu->pstate.V ? 'V' : '-');
    // Print non-zero memory
    printf("Non-zero memory:\n");
    for (uint32_t addr = 0; addr < MEMORY_SIZE; addr += ADDRESS_ALIGNMENT) {
        uint32_t value = read_memory(cpu, addr, ADDRESS_ALIGNMENT);
        if (value != 0) {
            printf("0x%08x: 0x%08x\n", addr, value);
        }
    }
}