#include "structures.h"
#include <string.h>

void initialise_cpu(CPU *cpu) {
    memset(cpu, 0, sizeof(CPU));
    cpu->pstate.Z = 1; // INITIAL PSTATE value with Z flag set
}

uint64_t read_register64(const CPU *cpu, uint8_t reg_index) {
    if (reg_index < NUM_REGISTERS) {
        return cpu->registers[reg_index];
    } else {
        return 0;
    }
}

uint32_t read_register32(const CPU* cpu, uint8_t reg_index) {
    if (reg_index < NUM_REGISTERS) {
        return (uint32_t)(cpu->registers[reg_index] & 0xFFFFFFFF); // Masks the lower 32 bits
    } else {
        return 0;
    }
}

void write_register64(CPU *cpu, uint8_t reg_index, uint64_t value) {
    if (reg_index < NUM_REGISTERS) {
        cpu->registers[reg_index] = value;
    }
}

void write_register32(CPU *cpu, uint8_t reg_index, uint32_t value) {
    if (reg_index < NUM_REGISTERS) {
        cpu->registers[reg_index] = (uint64_t)value;
    }
}

uint64_t read_memory(const CPU *cpu, uint64_t address) {
    if (address < MEMORY_SIZE) {
        return cpu->memory[address];
    } else {
        return 0;
    }
}

void write_memory(CPU *cpu, uint64_t address, uint64_t value) {
    if (address < MEMORY_SIZE) {
        cpu->memory[address] = value;
    }
}