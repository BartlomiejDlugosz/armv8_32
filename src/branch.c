#include <stdlib.h>
#include <stdint.h>
#include "structures.h"
#include "branch.h"
#include <stdbool.h>
#include <stdio.h>
#define addsimm19 (*cpu).PC += simm19

void branchExecute(uint64_t current_instr, CPU *cpu) {
    // first check if unconditional or not
    uint64_t bitmapped = current_instr & 0x40000000;
    if(bitmapped == 0x40000000) {
        // check for register based
        if(current_instr & 0x80000000 == 0x80000000) {
            uint8_t xn = (current_instr >> 5) & 31;
            if (xn == 31)
            {
                printf("eroor: xzr encoding for branch instruction");
            } else {
                (*cpu).PC = read_register64(&cpu, xn);
            }
        } else {
            // conditional branching
            uint64_t simm19 = (current_instr & 0xFFFFE0) >> 3;
            if (simm19 & 0x100000 == 0x100000) {
                // sign extend
                simm19 = simm19 | 0xFFFFFFFFFFF00000;
            }
            switch (current_instr & 0xF) {
                case 0: // eq
                    if ((*cpu).pstate.Z == 1) {
                        addsimm19;
                    }
                    break;
                case 1: // not eq
                    if ((*cpu).pstate.Z == 0) {
                        addsimm19;
                    }
                    break;
                case 10:  // signed >=
                    if ((*cpu).pstate.N == (*cpu).pstate.V) {
                        addsimm19;
                    }
                    break;
                case 11: // signed <
                    if ((*cpu).pstate.N != (*cpu).pstate.V) {
                        addsimm19;
                    }
                    break;
                case 12: // signed <=
                    if ((*cpu).pstate.Z == 0 && (*cpu).pstate.N == (*cpu).pstate.V) {
                        addsimm19;
                    }
                    break;
                case 13: // signed >
                    if (!((*cpu).pstate.Z == 0 && (*cpu).pstate.N == (*cpu).pstate.V)) {
                        addsimm19;
                    }
                    break;
                case 14: // unconditional
                    addsimm19;
                    break;
            }
        }
    } else {
        // unconditional jump
        uint64_t simm26 = (current_instr & 0x3FFFFFF) << 2;
        if (simm26 & 0x08000000 == 0x08000000) {
            // sign extend
            simm26 = simm26 | 0xFFFFFFF000000000;
        }
        (*cpu).PC += simm26;
    }
}
