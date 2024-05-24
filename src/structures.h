#include <stdint.h>

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
