#include <stdlib.h>
#include <stdint.h>
#include "structures.h"

extern CPU cpu;
extern read_register64(CPU, uint8_t)
void branchExecute(uint64_t current_instr) {
    uint64_t bitmapped = current_instr & 0x40000000;
    if(bitmapped == 0x40000000) {
        if(current_instr & 0x80000000 == 0x80000000) {
            uint8_t xn = (current_instr & 3E0) >> 5;
            cpu->PC = read_register64(cpu, xn);
        } else {
            uint64_t simm19 = (current_instr & 0xFFFFE0) >> 3;
            if (simm19 & 0x100000 = 0x100000) {
                simm19 = simm19 | 0xFFFFFFFFFFF00000;
            }
            switch (current_instr & 0xF) {

            }
        }
    }
}