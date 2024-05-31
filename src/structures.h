#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "includes.h"

#define NUM_REGISTERS 31
#define MEMORY_SIZE (2 * 1024 * 1024)  // 2MB

#define PRINT_INVALID_REGISTER_INDEX(reg_index)               \
  fprintf(stderr, "Invalid register index: %u\n", reg_index); \
  exit(EXIT_FAILURE);

#define PRINT_INVALID_MEMORY_ACCESS(address)                              \
  fprintf(stderr, "Invalid memory access at address: 0x%08x\n", address); \
  exit(EXIT_FAILURE);

// PSTATE struct
typedef struct {
  uint8_t N;  // Negative condition flag
  uint8_t Z;  // Zero condition flag
  uint8_t C;  // Carry condition flag
  uint8_t V;  // Overflow condition flag
} PSTATE;

// CPU struct
typedef struct {
  uint64_t registers[NUM_REGISTERS];
  uint64_t ZR;                  // Zero register
  uint64_t PC;                  // Program counter
  PSTATE pstate;                // Program state register
  uint8_t memory[MEMORY_SIZE];  // Memory
} CPU;

void initialise_cpu(CPU *cpu);

uint64_t read_register64(const CPU *cpu, uint8_t reg_index);

uint32_t read_register32(const CPU *cpu, uint8_t reg_index);

void write_register64(CPU *cpu, uint8_t reg_index, uint64_t value);

void write_register32(CPU *cpu, uint8_t reg_index, uint32_t value);

uint64_t read_memory(const CPU *cpu, uint32_t address, uint8_t num_bytes);

void write_memory(CPU *cpu, uint32_t address, uint64_t value,
                  uint8_t num_bytes);

void print_cpu_state(const CPU *cpu);

#endif  // STRUCTURES_H
