#include "includes.h"

#define addsimm19                    \
    cpu->PC += (br_cond.simm19 * 4); \
    cpu->PC -= 4;

void branchExecute(CPU *cpu, uint32_t instruction) {
    // first check if unconditional or not
    union branch_instruction instr =
        (union branch_instruction){.bits = instruction};
    if (instr.type == 1) {
        // check for register based
        if (instr.reg == 1) {
            union branch_register br_reg =
                (union branch_register){.bits = instr.simm26};
            if (br_reg.xn == 31) {
                printf("error: xzr encoding for branch instruction");
            } else {
                cpu->PC = read_register64(cpu, br_reg.xn);
                cpu->PC -= 4;
            }
        } else {
            // conditional branching
            union branch_conditional br_cond =
                (union branch_conditional){.bits = instr.simm26};
            switch (br_cond.cond) {
                case 0:  // eq
                    if (cpu->pstate.Z == 1) {
                        addsimm19;
                    }
                    break;
                case 1:  // not eq
                    if (cpu->pstate.Z == 0) {
                        addsimm19;
                    }
                    break;
                case 10:  // signed >=
                    if (cpu->pstate.N == cpu->pstate.V) {
                        addsimm19;
                    }
                    break;
                case 11:  // signed <
                    if (cpu->pstate.N != cpu->pstate.V) {
                        addsimm19;
                    }
                    break;
                case 12:  // signed <=
                    if (cpu->pstate.Z == 0 && cpu->pstate.N == cpu->pstate.V) {
                        addsimm19;
                    }
                    break;
                case 13:  // signed >
                    if (!(cpu->pstate.Z == 0 &&
                          cpu->pstate.N == cpu->pstate.V)) {
                        addsimm19;
                    }
                    break;
                case 14:  // unconditional
                    addsimm19;
                    break;
            }
        }
    } else {
        // unconditional jump
        cpu->PC += instr.simm26 * 4;
        cpu->PC -= 4;
    }
}
