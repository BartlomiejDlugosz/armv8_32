#include "includes.h"

//#define INT_MAX ((uint32_t)(-1))
//#define INT_MIN ((int32_t)(INT_MAX) + 1)
//#define UINT_MAX ((uint32_t)(-1))

// Checking for unsigned overflow from
// https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
// if (x > 0 && a > INT_MAX - x) // `a + x` would overflow (1)
// if (x < 0 && a < INT_MIN - x) // `a + x` would underflow (2)
//
// if (x < 0 && a > INT_MAX + x) // `a - x` would overflow (3)
// if (x > 0 && a < INT_MIN + x) // `a - x` would underflow (4)
//
// in a similar fashion we can check unsigned overflow by using
// UINT_MAX, as well as extending to 64-bit with LONG
// Notice that for unsigned arithmetic cases (2) and (3) cannot
// occur since x < 0 is impossible.
//
// NOTE: rn_contents, op2 are assumed to be unsigned!
//
// 32-bit
// signed
#define add_overflow_32       \
    (((int32_t)op2) > 0 &&    \
     ((int32_t)rn_contents) > \
         INT_MAX - ((int32_t)op2))  // `rn_contents + op2` would overflow (1)
#define add_underflow_32      \
    (((int32_t)op2) < 0 &&    \
     ((int32_t)rn_contents) < \
         INT_MIN - ((int32_t)op2))  // `rn_contents + op2` would underflow (2)
#define sub_overflow_32       \
    (((int32_t)op2) < 0 &&    \
     ((int32_t)rn_contents) > \
         INT_MAX + ((int32_t)op2))  // `rn_contents - op2` would overflow (3)
#define sub_underflow_32      \
    (((int32_t)op2) > 0 &&    \
     ((int32_t)rn_contents) < \
         INT_MIN + ((int32_t)op2))  // `rn_contents - op2` would underflow (4)
// unsigned
#define uadd_overflow_32                 \
    (op2 > 0 &&                          \
     rn_contents > (uint32_t)(UINT_MAX - \
                              op2))  // `rn_contents + op2` would overflow (1)
#define usub_underflow_32 \
    (op2 > 0 && rn_contents < op2)  // `rn_contents - op2` would underflow (4)

// 64-bit
// signed
#define add_overflow_64       \
    (((int64_t)op2) > 0 &&    \
     ((int64_t)rn_contents) > \
         LONG_MAX - ((int64_t)op2))  // `rn_contents + op2` would overflow (1)
#define add_underflow_64      \
    (((int64_t)op2) < 0 &&    \
     ((int64_t)rn_contents) < \
         LONG_MIN -           \
             ((int64_t)op2))  // `rn_contents + op2` would underflow (2)
#define sub_overflow_64       \
    (((int64_t)op2) < 0 &&    \
     ((int64_t)rn_contents) > \
         LONG_MAX + ((int64_t)op2))  // `rn_contents - op2` would overflow (3)
#define sub_underflow_64      \
    (((int64_t)op2) > 0 &&    \
     ((int64_t)rn_contents) < \
         LONG_MIN +           \
             ((int64_t)op2))  // `rn_contents - op2` would underflow (4)
// unsigned
#define uadd_overflow_64                  \
    (op2 > 0 &&                           \
     rn_contents > (uint64_t)(ULONG_MAX - \
                              op2))  // `rn_contents + op2` would overflow (1)
#define usub_underflow_64 \
    (op2 > 0 && rn_contents < op2)  // `rn_contents - op2` would underflow (4)

#define extract_msb_shift_64 63
#define extract_msb_shift_32 31

#define stack_pointer_encoding 0b11111

// Handle immediate instructions
uint64_t arithmetic_helper_64(CPU *cpu, unsigned opc, uint64_t rn_contents,
                              uint64_t op2) {
    uint64_t result;
    switch (opc) {
        case 0b00:  // add
            result = rn_contents + op2;
            break;
        case 0b01:  // adds
            result = rn_contents + op2;
            cpu->pstate.N =
                (uint8_t)(result >>
                          extract_msb_shift_64);  // extract MSB (sign bit)
            cpu->pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            cpu->pstate.C = (uint8_t)(uadd_overflow_64 ? 1 : 0);
            cpu->pstate.V =
                (uint8_t)(add_overflow_64 || add_underflow_64 ? 1 : 0);
            break;
        case 0b10:  // sub
            result = rn_contents - op2;
            break;
        case 0b11:  // subs
            result = rn_contents - op2;
            cpu->pstate.N =
                (uint8_t)(result >>
                          extract_msb_shift_64);  // extract MSB (sign bit)
            cpu->pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            cpu->pstate.C = (uint8_t)(usub_underflow_64 ? 0 : 1);
            cpu->pstate.V =
                (uint8_t)(sub_underflow_64 || sub_overflow_64 ? 1 : 0);
            break;
        default:
            OPC_CASE_ERROR();
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
            cpu->pstate.N =
                (uint8_t)(result >>
                          extract_msb_shift_32);  // extract MSB (sign bit)
            cpu->pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            cpu->pstate.C = (uint8_t)(uadd_overflow_32 ? 1 : 0);
            cpu->pstate.V =
                (uint8_t)(add_overflow_32 || add_underflow_32 ? 1 : 0);
            break;
        case 0b10:  // sub
            result = rn_contents - op2;
            break;
        case 0b11:  // subs
            result = rn_contents - op2;
            cpu->pstate.N =
                (uint8_t)(result >>
                          extract_msb_shift_32);  // extract MSB (sign bit)
            cpu->pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            cpu->pstate.C = (uint8_t)(usub_underflow_32 ? 0 : 1);
            cpu->pstate.V =
                (uint8_t)(sub_underflow_32 || sub_overflow_32 ? 1 : 0);
            break;
        default:
            OPC_CASE_ERROR();
    }
    return result;
}

void arithmetic_immediate_64(CPU *cpu, union data_processing_instruction instr,
                             union data_processing_data_immediate data,
                             union arithmetic_immediate_operand operand) {
    // This function performs arithmetic with 64-bit registers
    uint64_t op2 = (uint64_t)(operand.sh == 1 ? ((uint64_t)operand.imm12) << 12
                                              : operand.imm12);

    assert(operand.rn != stack_pointer_encoding);  // not handling stack pointer
    assert(instr.rd != stack_pointer_encoding ||
           !(instr.opc == 0b00 ||
             instr.opc == 0b10));  // not handling stack pointer

    uint64_t rn_contents = read_register64(cpu, operand.rn);
    uint64_t result = arithmetic_helper_64(cpu, instr.opc, rn_contents, op2);
    write_register64(cpu, instr.rd, result);
    return;
}

void arithmetic_immediate_32(CPU *cpu, union data_processing_instruction instr,
                             union data_processing_data_immediate data,
                             union arithmetic_immediate_operand operand) {
    // This function performs arithmetic with 32-bit registers
    uint32_t op2 = (uint32_t)(operand.sh == 1 ? ((uint64_t)operand.imm12) << 12
                                              : operand.imm12);

    assert(operand.rn != stack_pointer_encoding);  // not handling stack pointer
    assert(instr.rd != stack_pointer_encoding ||
           !(instr.opc == 0b00 ||
             instr.opc == 0b10));  // not handling stack pointer

    uint32_t rn_contents = read_register32(cpu, operand.rn);
    uint32_t result = arithmetic_helper_32(cpu, instr.opc, rn_contents, op2);
    write_register32(cpu, instr.rd, result);
    return;
}

void wide_move_64(CPU *cpu, union data_processing_instruction instr,
                  union data_processing_data_immediate data,
                  union wide_move_operand operand) {
    uint64_t shift = operand.hw * 16;
    uint64_t op = ((uint64_t)operand.imm16) << shift;
    uint64_t result;
    uint64_t current_rd = read_register64(cpu, instr.rd);
    switch (instr.opc) {
        case 0b00:
            result = ~op;
            break;
        case 0b10:
            result = op;
            break;
        case 0b11:
            // keep_mask: 16 bits set to one where imm16 is to be inserted
            uint64_t keep_mask = ((uint64_t)0xFFFF) << shift;
            current_rd = current_rd & (~keep_mask);  // clear bits
            result = current_rd | op;  // set bits using shifted imm16
            break;
        default:
            OPC_CASE_ERROR();
    }
    write_register64(cpu, instr.rd, result);
    return;
}

void wide_move_32(CPU *cpu, union data_processing_instruction instr,
                  union data_processing_data_immediate data,
                  union wide_move_operand operand) {
    assert(operand.hw == 0b00 ||
           operand.hw == 0b01);  // can only shift by 0 or 16 bits

    uint32_t shift = operand.hw * 16;  // pre: hw < 2
    uint32_t op = ((uint32_t)operand.imm16) << shift;
    uint32_t result;
    uint32_t current_rd = read_register32(cpu, instr.rd);

    switch (instr.opc) {
        case 0b00:
            result = ~op;
            break;
        case 0b10:
            result = op;
            break;
        case 0b11:
            // keep_mask: 16 bits set to one where imm16 is to be inserted
            uint32_t keep_mask = ((uint32_t)0xFFFF) << shift;
            current_rd = current_rd & (~keep_mask);  // clear bits
            result = current_rd | op;  // set bits using shifted imm16
            break;
        default:
            OPC_CASE_ERROR();
    }

    write_register32(cpu, instr.rd, result);
    return;
}

// End of immediate instructions

// Handle register instructions

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
        default:
            SHIFT_TYPE_DETERMINATION_ERROR();
    }

    assert(instr.rd != stack_pointer_encoding ||
           !(instr.opc == 0b00 || instr.opc == 0b10));

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
            SHIFT_TYPE_DETERMINATION_ERROR();
    }

    assert(instr.rd != stack_pointer_encoding ||
           !(instr.opc == 0b00 || instr.opc == 0b10));

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
        case 0b11:
            uint64_t op2_left = op2 << (64 - data.operand);
            uint64_t op2_right = op2 >> data.operand;
            op2 = op2_left | op2_right;
            break;
        default:
            SHIFT_TYPE_DETERMINATION_ERROR();
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
            cpu->pstate.N =
                (uint8_t)(result >>
                          extract_msb_shift_64);  // extract MSB (sign bit)
            cpu->pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            cpu->pstate.C = (uint8_t)0;
            cpu->pstate.V = (uint8_t)0;
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
        case 0b11:
            uint32_t op2_left = op2 << (32 - data.operand);
            uint32_t op2_right = op2 >> data.operand;
            op2 = op2_left | op2_right;
            break;
        default:
            SHIFT_TYPE_DETERMINATION_ERROR();
    }
    // check if we need to bitwise negate op2
    if (opr.maybe_N == 1) {
        op2 = ~op2;
    }

    uint32_t rn_contents = read_register32(cpu, data.rn);
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
            cpu->pstate.N =
                (uint8_t)(result >>
                          extract_msb_shift_32);  // extract MSB (sign bit)
            cpu->pstate.Z = (uint8_t)(result == 0 ? 1 : 0);
            cpu->pstate.C = (uint8_t)0;
            cpu->pstate.V = (uint8_t)0;
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

    result = read_register64(cpu, operand.ra) + ((uint64_t)rnrm);
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

    uint32_t result = read_register32(cpu, operand.ra) + ((uint32_t)rnrm);
    write_register32(cpu, instr.rd, result);
    return;
}

// End of register instructions

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
        UNDEFINED_INSTRUCTION();
    }
    return;
}

void perform_data_processing_register(
    CPU *cpu, union data_processing_instruction instr,
    union data_processing_data_register data) {
    // utilitising early returns to avoid nested if-else
    if (instr.maybe_M == 1) {
        union multiply_operand operand =
            (union multiply_operand){.bits = data.operand};

        if (instr.sf == 1) {
            multiply_64(cpu, instr, data, operand);
        } else {
            multiply_32(cpu, instr, data, operand);
        }
        return;
    }

    union arithmetic_logic_opr opr =
        (union arithmetic_logic_opr){.bits = data.opr};
    if (opr.type == 1) {
        if (instr.sf == 1) {
            arithmetic_register_64(cpu, instr, data, opr);
        } else {
            arithmetic_register_32(cpu, instr, data, opr);
        }
        return;
    }

    if (instr.sf == 1) {
        logic_64(cpu, instr, data, opr);
        return;
    }

    logic_32(cpu, instr, data, opr);
    return;
}

void data_processing_init(CPU *cpu, uint32_t instruction, bool is_immediate) {
    union data_processing_instruction instr =
        (union data_processing_instruction){.bits = instruction};

    if (is_immediate) {
        union data_processing_data_immediate data =
            (union data_processing_data_immediate){.bits = instr.data};
        perform_data_processing_immediate(cpu, instr, data);
        return;
    }
    union data_processing_data_register data =
        (union data_processing_data_register){.bits = instr.data};
    perform_data_processing_register(cpu, instr, data);
    return;
};
