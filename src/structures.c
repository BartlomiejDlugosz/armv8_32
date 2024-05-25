#include "structures.h"
#include <string.h>
#include <stdio.h>

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

uint32_t read_memory(const CPU *cpu, uint32_t address) {
    if (address < MEMORY_SIZE) {
        return cpu->memory[address];
    } else {
        return 0;
    }
}

void write_memory(CPU *cpu, uint32_t address, uint32_t value) {
    if (address < MEMORY_SIZE) {
        cpu->memory[address] = value;
    }
}

void print_cpu_state(const CPU *cpu) {
    printf("Registers:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("X%02d = %016lx\n", i, cpu->registers[i]);
    }
    printf("PC = %016lx\n", cpu->PC);
    printf("PSTATE: %c%c%c%c\n",
            cpu->pstate.N ? 'N' : '-',
            cpu->pstate.Z ? 'Z' : '-',
            cpu->pstate.C ? 'C' : '-',
            cpu->pstate.V ? 'V' : '-');
    printf("Non-zero memory:\n");
    for (uint32_t addr = 0; addr < MEMORY_SIZE; addr += 4) {
        uint32_t value = read_memory(cpu, addr);
        if (value != 0) {
            printf("0x%08x: 0x%08x\n", addr, value);
        }
    }
}