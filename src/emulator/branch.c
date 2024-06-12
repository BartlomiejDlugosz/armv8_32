#include "includes.h"

#define addsimm19                    \
    cpu->PC += (br_cond.simm19 * 4); \
    cpu->PC -= 4;

#define XN_REG_LOCATION 5
#define SIMM19_COND_LOCATION 5
#define TYPE_INSTR_LOCATION 30
#define REG_INSTR_LOCATION 31

branch_register create_branch_register(uint32_t bits) {
    branch_register instr;
    instr.xn = (bits >> XN_REG_LOCATION);
    return instr;
}

branch_conditional create_branch_conditional(uint32_t bits) {
    branch_conditional instr;
    instr.cond = bits;
    instr.simm19 = (bits >> SIMM19_COND_LOCATION);
    return instr;
}

branch_instruction create_branch_instruction(uint32_t bits) {
    branch_instruction instr;
    instr.simm26 = bits;
    instr.type = (bits >> TYPE_INSTR_LOCATION);
    instr.reg = (bits >> REG_INSTR_LOCATION);
    return instr;
}

void branchExecute(CPU *cpu, uint32_t instruction) {
    // first check if unconditional or not
    branch_instruction instr = create_branch_instruction(instruction);
    if (instr.type == 1) {
        // check for register based
        if (instr.reg == 1) {
            branch_register br_reg = create_branch_register(instr.simm26);
            if (br_reg.xn == 31) {
                INVALID_XZR_ENCODING();
            } else {
                cpu->PC = read_register64(cpu, br_reg.xn);
                cpu->PC -= 4;
            }
        } else {
            // conditional branching
            branch_conditional br_cond =
                create_branch_conditional(instr.simm26);
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
