#include "data_processing.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
// #include <limits.h> // for INT_MAX, although I should probably hardcode the
// values for the architecture

// Checking for unsigned overflow
// from
// https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
// if (x > 0 && a > INT_MAX - x) // `a + x` would overflow (1)
// if (x < 0 && a < INT_MIN - x) // `a + x` would underflow (2)
// if (x < 0 && a > INT_MAX + x) // `a - x` would overflow (3)
// if (x > 0 && a < INT_MIN + x) // `a - x` would underflow (4)
// the check for x > 0 (or in our case op2 > 0) is ommitted
// since this is given. This also means that case
// (2) and (3) above are not possible.
// Hence, addition cannot underflow and subtraction cannot overflow
//
#define add_overflow_32 \
    (rn_contents > INT_MAX - op2)  // `rn + op2` will overflow (signed)
#define sub_overflow_32 \
    (rn_contents < INT_MIN + op2)  // `rn - op2` will underflow (signed)
#define uadd_overflow_32 \
    (rn_contents >       \
     UINT_MAX - op2)  // `rn + op2` will produce a carry (unsigned)
#define usub_overflow_32 \
    (rn_contents < op2)  // `rn - op2` will produce a borrow (unsigned)

// we need to adapt the MAX/MIN values for 64-bit
#define add_overflow_64 \
    (rn_contents > LONG_MAX - op2)  // `rn + op2` will overflow (signed)
#define sub_overflow_64 \
    (rn_contents < LONG_MIN + op2)  // `rn - op2` will underflow (signed)
#define uadd_overflow_64 \
    (rn_contents >       \
     ULONG_MAX - op2)  // `rn + op2` will produce a carry (unsigned)
#define usub_overflow_64 \
    (rn_contents < op2)  // `rn - op2` will produce a borrow (unsigned)
// TODO: define MAX/MIN manually, since they are dependent on platform!

// Handle immediate instructions
void perform_data_processing_immediate(
    CPU *cpu, union data_processing_instruction instr,
    union data_processing_data_immediate data) {
    if (data.opi == 2) {
        union arithmetic_immediate_operand operand =
            (union arithmetic_immediate_operand){.bits = instr.data};
        if (instr.sf == 1) {
            arithmetic_immediate_64(cpu, instr, data, operand);
        } else {
            arithmetic_immediate_32(cpu, instr, data, operand);
        }
    } else if (data.opi == 5) {
        union wide_move_operand operand =
            (union wide_move_operand){.bits = instr.data};
        if (instr.sf == 1) {
            wide_move_64(cpu, instr, data, operand);
        } else {
            wide_move_32(cpu, instr, data, operand);
        }
    } else {
        printf("undefined instruction\n");
    }
    return;
}

uint64_t arithmetic_helper_64(CPU *cpu, unsigned opc, uint64_t rn_contents,
                              uint64_t op2) {
    uint64_t result;
    switch (opc) {
        case 0b00:  // add
            result = rn_contents + op2;
            break;
        case 0b01:  // adds
            result = rn_contents + op2;
            (*cpu).pstate.N =
                (uint8_t)(result >> 63);  // extract MSB (sign bit)
            (*cpu).pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            (*cpu).pstate.C = (uint8_t)(uadd_overflow_64 ? 1 : 0);
            (*cpu).pstate.V = (uint8_t)(add_overflow_64 ? 1 : 0);
            break;
        case 0b10:  // sub
            result = rn_contents - op2;
            break;
        case 0b11:  // subs
            result = rn_contents - op2;
            (*cpu).pstate.N =
                (uint8_t)(result >> 63);  // extract MSB (sign bit)
            (*cpu).pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            (*cpu).pstate.C = (uint8_t)(usub_overflow_64 ? 1 : 0);
            (*cpu).pstate.V = (uint8_t)(sub_overflow_64 ? 1 : 0);
            break;
        default:
            printf("Something has gone wrong in opc case!\n");
    }
    return result;
}

uint32_t arithmetic_helper_32(CPU *cpu, unsigned opc, uint32_t rn_contents,
                              uint32_t op2) {
    uint32_t result;
    switch (opc) {
        case 0b00:  // add
            result = rn_contents + op2;
            break;
        case 0b01:  // adds
            result = rn_contents + op2;
            (*cpu).pstate.N =
                (uint8_t)(result >> 31);  // extract MSB (sign bit)
            (*cpu).pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            (*cpu).pstate.C = (uint8_t)(uadd_overflow_32 ? 1 : 0);
            (*cpu).pstate.V = (uint8_t)(add_overflow_32 ? 1 : 0);
            break;
        case 0b10:  // sub
            result = rn_contents - op2;
            break;
        case 0b11:  // subs
            result = rn_contents - op2;
            (*cpu).pstate.N =
                (uint8_t)(result >> 31);  // extract MSB (sign bit)
            (*cpu).pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            (*cpu).pstate.C = (uint8_t)(usub_overflow_32 ? 1 : 0);
            (*cpu).pstate.V = (uint8_t)(sub_overflow_32 ? 1 : 0);
            break;
        default:
            printf("Something has gone wrong in opc case!\n");
    }
    return result;
}

void arithmetic_immediate_64(CPU *cpu, union data_processing_instruction instr,
                             union data_processing_data_immediate data,
                             union arithmetic_immediate_operand operand) {
    // This function performs arithmetic with 64-bit registers
    uint64_t op2 =
        (uint64_t)(operand.sh == 1 ? operand.imm12 << 12 : operand.imm12);

    if (operand.rn == 0b11111) {
        // rn
        printf("error: reached a case we are not handling!\n");
        printf("error: rn is stack pointer!\n");
        return;
    }

    if (instr.rd == 0b11111 && (instr.opc == 0b00 || instr.opc == 0b10)) {
        // rd encodes the stack pointer, not handled
        printf("error: reached a case we are not handling!\n");
        printf("error: rn is stack pointer!\n");
        return;
    };

    uint64_t rn_contents = read_register64(cpu, operand.rn);
    uint64_t result = arithmetic_helper_64(cpu, instr.opc, rn_contents, op2);
    write_register64(cpu, instr.rd, result);
    return;
}

void arithmetic_immediate_32(CPU *cpu, union data_processing_instruction instr,
                             union data_processing_data_immediate data,
                             union arithmetic_immediate_operand operand) {
    // This function performs arithmetic with 32-bit registers
    uint32_t op2 =
        (uint32_t)(operand.sh == 1 ? operand.imm12 << 12 : operand.imm12);

    if (operand.rn == 0b11111) {
        // rn
        printf("error: reached a case we are not handling!\n");
        printf("error: rn is stack pointer!\n");
        return;
    }

    if (instr.rd == 0b11111 && (instr.opc == 0b00 || instr.opc == 0b10)) {
        // rd encodes the stack pointer, not handled
        printf("error: reached a case we are not handling!\n");
        printf("error: rn is stack pointer!\n");
        return;
    };

    uint32_t rn_contents = read_register32(cpu, operand.rn);
    uint64_t result = arithmetic_helper_32(cpu, instr.opc, rn_contents, op2);
    write_register32(cpu, instr.rd, result);
    return;
}

void wide_move_64(CPU *cpu, union data_processing_instruction instr,
                  union data_processing_data_immediate data,
                  union wide_move_operand operand) {
    uint64_t shift = operand.hw * 16;
    uint64_t op = operand.imm16 << shift;
    uint64_t result;
    uint64_t current_rd = read_register64(cpu, instr.rd);

    switch (instr.opc) {
        case 0b00:
            result = ~op;
            break;
        // case 0b01:
        //	printf("something went wrong in opc case!"); fall through to
        //default
        case 0b10:
            result = op;
            break;
        case 0b11:
            // keep_mask: 16 bits set to one where imm16 is to be inserted
            uint64_t keep_mask = 0xFFFF << shift;
            current_rd = current_rd && (~keep_mask);  // clear bits
            result = current_rd || op;  // set bits using shifted imm16
            break;
        default:
            printf("something went wrong in opc case!\n");
            break;
    }

    write_register64(cpu, instr.rd, result);
    return;
}

void wide_move_32(CPU *cpu, union data_processing_instruction instr,
                  union data_processing_data_immediate data,
                  union wide_move_operand operand) {
    if (operand.hw > 0b01) {
        // shift is too large for 32-bit register
        printf("error: shift is too large for register size!\n");
        return;
    }

    uint32_t shift = operand.hw * 16;  // pre: hw < 2
    uint32_t op = operand.imm16 << shift;
    uint32_t result;
    uint32_t current_rd = read_register32(cpu, instr.rd);

    switch (instr.opc) {
        case 0b00:
            result = ~op;
            break;
        // case 0b01:
        //	printf("something went wrong in opc case!"); fall through to
        //default
        case 0b10:
            result = op;
            break;
        case 0b11:
            // keep_mask: 16 bits set to one where imm16 is to be inserted
            uint32_t keep_mask = 0xFFFF << shift;
            current_rd = current_rd && (~keep_mask);  // clear bits
            result = current_rd || op;  // set bits using shifted imm16
            break;
        default:
            printf("something went wrong in opc case!\n");
            break;
    }

    write_register32(cpu, instr.rd, result);
    return;
}

// End of immediate instructions

// Handle register instructions

void perform_data_processing_register(
    CPU *cpu, union data_processing_instruction instr,
    union data_processing_data_register data) {
    if (instr.maybe_M == 1) {
        union multiply_operand operand =
            (union multiply_operand){.bits = data.operand};

        if (instr.sf == 1) {
            multiply_64(cpu, instr, data, operand);
        } else {
            multiply_32(cpu, instr, data, operand);
        }
    } else {
        union arithmetic_logic_opr opr =
            (union arithmetic_logic_opr){.bits = data.opr};
        if (opr.type == 1) {
            if (instr.sf == 1) {
                arithmetic_register_64(cpu, instr, data, opr);
            } else {
                arithmetic_register_64(cpu, instr, data, opr);
            }
        } else {
            if (instr.sf == 1) {
                logic_64(cpu, instr, data, opr);
            } else {
                logic_32(cpu, instr, data, opr);
            }
        }
    }
    return;
}

void arithmetic_register_64(CPU *cpu, union data_processing_instruction instr,
                            union data_processing_data_register data,
                            union arithmetic_logic_opr opr) {
    uint64_t op2 = read_register64(cpu, data.rm);
    switch (opr.shift) {
        case 0b00:
            op2 = op2 << data.operand;
            break;
        case 0b01:
            op2 = op2 >> data.operand;
            break;
        case 0b10:
            // convert to signed int, so that
            // >> performs arithmetic shift
            // see: https://stackoverflow.com/a/7632
            int64_t op2_signed = (int64_t)op2;
            op2 = (uint64_t)(op2_signed >> data.operand);
            break;
        // case 0b11:
        //	break; shouldn't occur, fall through to default
        default:
            printf("error in determining shift type for op2!\n");
    }

    if (instr.rd == 0b11111 && (instr.opc == 0b00 || instr.opc == 0b10)) {
        // rd encodes the stack pointer, not handled
        printf("error: reached a case we are not handling!\n");
        printf("error: rn is stack pointer!\n");
        return;
    };

    uint64_t rn_contents = read_register64(cpu, data.rn);
    uint64_t result = arithmetic_helper_64(cpu, instr.opc, rn_contents, op2);
    write_register64(cpu, instr.rd, result);
    return;
}

void arithmetic_register_32(CPU *cpu, union data_processing_instruction instr,
                            union data_processing_data_register data,
                            union arithmetic_logic_opr opr) {
    uint32_t op2 = read_register32(cpu, data.rm);
    switch (opr.shift) {
        case 0b00:
            op2 = op2 << data.operand;
            break;
        case 0b01:
            op2 = op2 >> data.operand;
            break;
        case 0b10:
            // convert to signed int, so that
            // >> performs arithmetic shift
            // see: https://stackoverflow.com/a/7632
            int32_t op2_signed = (int32_t)op2;
            op2 = (uint32_t)(op2_signed >> data.operand);
            break;
        // case 0b11:
        //	break; shouldn't occur, fall through to default
        default:
            printf("error in determining shift type for op2!\n");
    }

    if (instr.rd == 0b11111 && (instr.opc == 0b00 || instr.opc == 0b10)) {
        // rd encodes the stack pointer, not handled
        printf("error: reached a case we are not handling!\n");
        printf("error: rn is stack pointer!\n");
        return;
    };

    uint32_t rn_contents = read_register32(cpu, data.rn);
    uint32_t result = arithmetic_helper_32(cpu, instr.opc, rn_contents, op2);
    write_register32(cpu, instr.rd, result);
    return;
}

void logic_64(CPU *cpu, union data_processing_instruction instr,
              union data_processing_data_register data,
              union arithmetic_logic_opr opr) {
    uint64_t op2 = read_register64(cpu, data.rm);
    switch (opr.shift) {
        case 0b00:
            op2 = op2 << data.operand;
            break;
        case 0b01:
            op2 = op2 >> data.operand;
            break;
        case 0b10:
            // convert to signed int, so that
            // >> performs arithmetic shift
            // see: https://stackoverflow.com/a/7632
            int64_t op2_signed = (int64_t)op2;
            op2 = (uint64_t)(op2_signed >> data.operand);
            break;
        // case 0b11:
        //	break; shouldn't occur, fall through to default
        default:
            printf("error in determining shift type for op2!\n");
    }

    // check if we need to bitwise negate op2
    if (opr.maybe_N == 1) {
        op2 = ~op2;
    }
    uint64_t rn_contents = read_register64(cpu, data.rn);
    uint64_t result;
    switch (instr.opc) {
        case 0b00:  // and
            result = rn_contents & op2;
            break;
        case 0b01:  // or
            result = rn_contents | op2;
            break;
        case 0b10:  // and
            result = rn_contents ^ op2;
            break;
        case 0b11:  // and, setting flags
            result = rn_contents & op2;
            (*cpu).pstate.N =
                (uint8_t)(result >> 63);  // extract MSB (sign bit)
            (*cpu).pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            (*cpu).pstate.C = (uint8_t)0;
            (*cpu).pstate.V = (uint8_t)0;
            break;
    }

    write_register64(cpu, instr.rd, result);
    return;
}

void logic_32(CPU *cpu, union data_processing_instruction instr,
              union data_processing_data_register data,
              union arithmetic_logic_opr opr) {
    uint32_t op2 = read_register32(cpu, data.rm);
    switch (opr.shift) {
        case 0b00:
            op2 = op2 << data.operand;
            break;
        case 0b01:
            op2 = op2 >> data.operand;
            break;
        case 0b10:
            // convert to signed int, so that
            // >> performs arithmetic shift
            // see: https://stackoverflow.com/a/7632
            int32_t op2_signed = (int32_t)op2;
            op2 = (uint32_t)(op2_signed >> data.operand);
            break;
        // case 0b11:
        //	break; shouldn't occur, fall through to default
        default:
            printf("error in determining shift type for op2!\n");
    }
    // check if we need to bitwise negate op2
    if (opr.maybe_N == 1) {
        op2 = ~op2;
    }

    uint32_t rn_contents = read_register64(cpu, data.rn);
    uint32_t result;
    switch (instr.opc) {
        case 0b00:  // and
            result = rn_contents & op2;
            break;
        case 0b01:  // or
            result = rn_contents | op2;
            break;
        case 0b10:  // and
            result = rn_contents ^ op2;
            break;
        case 0b11:  // and, setting flags
            result = rn_contents & op2;
            (*cpu).pstate.N =
                (uint8_t)(result >> 31);  // extract MSB (sign bit)
            (*cpu).pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            (*cpu).pstate.C = (uint8_t)0;
            (*cpu).pstate.V = (uint8_t)0;
            break;
    }

    write_register32(cpu, instr.rd, result);
    return;
}

void multiply_64(CPU *cpu, union data_processing_instruction instr,
                 union data_processing_data_register data,
                 union multiply_operand operand) {
    uint64_t result;
    int64_t rnrm = (int64_t)(read_register64(cpu, data.rn) *
                             read_register64(cpu, data.rm));
    if (operand.x == 1) {
        rnrm = -rnrm;
    }
    result = read_register64(cpu, operand.ra) + ((unsigned)rnrm);
    write_register64(cpu, instr.rd, result);
    return;
}

void multiply_32(CPU *cpu, union data_processing_instruction instr,
                 union data_processing_data_register data,
                 union multiply_operand operand) {
    int32_t rnrm = (int32_t)(read_register32(cpu, data.rn) *
                             read_register32(cpu, data.rm));
    if (operand.x == 1) {
        rnrm = -rnrm;
    }

    uint32_t result = read_register64(cpu, operand.ra) + ((unsigned)rnrm);
    write_register32(cpu, instr.rd, result);
    return;
}

// End of register instructions

void data_processing_init(CPU *cpu, uint32_t instruction, bool is_immediate) {
    union data_processing_instruction instr =
        (union data_processing_instruction){.bits = instruction};
    if (is_immediate) {
        union data_processing_data_immediate data =
            (union data_processing_data_immediate){.bits = instr.data};
        perform_data_processing_immediate(cpu, instr, data);
    } else {
        union data_processing_data_register data =
            (union data_processing_data_register){.bits = instr.data};
        perform_data_processing_register(cpu, instr, data);
    }
};
