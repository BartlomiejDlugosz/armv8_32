#include <stdint.h>

#ifndef STRUCTURES
#define STRUCTURES

#define NUM_REGISTERS 31
#define MEMORY_SIZE (2 * 1024 * 1024) // 2MB

// PSTATE struct
typedef struct {
    uint8_t N; // Negative condition flag
    uint8_t Z; // Zero condition flag
    uint8_t C; // Carry condition flag
    uint8_t V; // Overflow condition flag
} PSTATE;

// CPU struct
typedef struct {
    uint64_t registers[NUM_REGISTERS];
    uint64_t ZR; // Zero register
    uint64_t PC; // Program counter
    PSTATE pstate; // Program state register
    uint8_t memory[MEMORY_SIZE]; // Memory
} CPU;

void initialise_cpu(CPU *cpu);

uint64_t read_register64(const CPU *cpu, uint8_t reg_index);

uint32_t read_register32(const CPU* cpu, uint8_t reg_index);

void write_register64(CPU *cpu, uint8_t reg_index, uint64_t value);

void write_register32(CPU *cpu, uint8_t reg_index, uint32_t value);

uint64_t read_memory(const CPU *cpu, uint32_t address, uint8_t num_bytes);

void write_memory(CPU *cpu, uint32_t address, uint64_t value, uint8_t num_bytes);

void print_cpu_state(const CPU *cpu);
#endif
