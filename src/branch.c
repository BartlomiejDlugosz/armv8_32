#include <stdlib.h>
#include <stdint.h>
#include "structures.h"
#include <stdbool.h>

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
                case 0:
                    if (cpu->PSTATE->Z == 1) {
                        cpu->PC += simm19;
                    }
                    break;
                case 1:
                    if (cpu->PSTATE->Z == 0) {
                        cpu->PC += simm19;
                    }
                    break;
                case 10:
                    if (cpu->PSTATE->N == cpu->PSTATE->V) {
                        cpu->PC += simm19;
                    }
                    break;
                case 11:
                    if (cpu->PSTATE->N != cpu->PSTATE->V) {
                        cpu->PC += simm19;
                    }
                    break;
                case 12:
                    if (cpu->PSTATE->Z == 0 && cpu->PSTATE->N == cpu->PSTATE->V) {
                        cpu->PC += simm19;
                    }
                    break;
                case 13:
                    if !(cpu->PSTATE->Z == 0 && cpu->PSTATE->N == cpu->PSTATE->V) {
                        cpu->PC += simm19;
                    }
                    break;
                case 14:
                    cpu->PC += simm19;
                    break;
            }
        }
    } else {
        uint64_t simm26 = (current_instr & 0x3FFFFFF) << 2;
        if (simm26 & 0x08000000 = 0x08000000) {
            simm26 = simm26 | 0xFFFFFFF000000000;
        }
        cpu->PC += simm26;
    }
}