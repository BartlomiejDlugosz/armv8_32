#include <stdbool.h>
#include "data_processing_assembly.h"
#include "../emulator/data_processing.h"
#include "register_to_number.h"

extern char *data_processing_opcodes[];

#define MULTIPLY_ADD "madd"
#define MULTIPLY_SUB "msub"
#define MULTIPLY "mul"
#define MULTIPLY_NEG "mneg"
#define ADD "add"
#define ADD_SIGNED "adds"
#define SUBTRACT "sub"
#define SUBTRACT_SIGNED "subs"
#define AND "and"
#define BIT_CLEAR "bic"
#define OR "orr"
#define OR_NOT "orn"
#define EXCLUSIVE_OR "eor"
#define EXCLUSIVE_OR_NOT "eon"
#define AND_SET "ands"
#define BIT_CLEAR_SET "bics"
#define MOVE_WIDE_NOT "movn"
#define MOVE_WIDE_ZERO "movz"
#define MOVE_WIDE_KEEP "movk"

static uint32_t resolve_operand(char *amount) {
    uint32_t result;
    if (sscanf(amount+1, "0x%x", &result) == 1 && amount[0] == '#') {
        return result;
    } else if (sscanf(amount+1, "%d", &result) == 1 && amount[0] == '#') {
        return result;
    } else {
        return register_to_number(amount);
    }
    fprintf(stderr, "operand not resolved\n");
    return 0;
}


static uint32_t generate_operand(multiply_operand *operand) {
    uint32_t result = 0;
    result |= ((uint32_t)operand->x) << 5;
    result |= (uint32_t)operand->ra;
    return result;
}

static uint32_t generate_arithmetic_operand(arithmetic_immediate_operand *arith_operand) {
    uint32_t result = 0;
    result |= ((uint32_t)arith_operand->sh) << 17;
    result |= ((uint32_t)arith_operand->imm12) << 5;
    result |= (uint32_t)arith_operand->rn;
    return result;
}

static uint32_t generate_wide_move_operand(wide_move_operand *operand) {
    uint32_t result = 0;
    result |= ((uint32_t)operand->hw) << 16;
    result |= (uint32_t)operand->imm16;
    return result;
}

static uint32_t generate_data_immediate_data(data_processing_data_immediate *data) {
    uint32_t result = 0;
    result |= ((uint32_t)data->opi) << 18;
    result |= (uint32_t)data->operand;
    return result;
}

static int32_t generate_data_register_data(data_processing_data_register *data) {
    uint32_t result = 0;
    result |= ((uint32_t)0b1) << 20;
    result |= ((uint32_t)data->opr) << 16;
    result |= ((uint32_t)data->rm) << 11;
    result |= ((uint32_t)data->operand) << 5;
    result |= (uint32_t)data->rn;
    return result;
}

static uint32_t generate_data_processing_instruction(data_processing_instruction *instr, bool is_register) {
    uint32_t result = 0;
    result |= ((uint32_t)instr->sf) << 31;
    result |= ((uint32_t)instr->opc) << 29;
    result |= ((uint32_t)instr->maybe_M) << 28;
    if (is_register) {
        result |= ((uint32_t)1) << 27;
    }
    result |= ((uint32_t)instr->data) << 5;
    result |= (uint32_t)instr->rd;
    return result;
}

static void madd(char *mulopcode, char *ra, multiply_operand *operand) {
    operand->x = 0;
    operand->ra = resolve_operand(ra);
    return;
}

static void msub(char *mulopcode, char *ra, multiply_operand *operand) {
    operand->x = 1;
    operand->ra = resolve_operand(ra);
    return;
}

static uint32_t wide_move_instruction (char *opcode, char *rd, char *imm, char *shift_type, char *shift_amount, bool register_64_bits) {
    wide_move_operand wide_operand;
    data_processing_instruction dpr_instruction;
    data_processing_data_immediate dpi_instruction;

    dpr_instruction.sf = register_64_bits;
    dpr_instruction.maybe_M = 1;
    dpi_instruction.opi = 0b101;
    wide_operand.imm16 = resolve_operand(imm);
    if (strcmp(opcode, MOVE_WIDE_NOT) == 0) {
        dpr_instruction.opc = 0b00;
    }
    else if (strcmp(opcode, MOVE_WIDE_ZERO) == 0) {
        dpr_instruction.opc = 0b10;
    }
    else if (strcmp(opcode, MOVE_WIDE_KEEP) == 0) {
        dpr_instruction.opc = 0b11;
    }
    if (shift_amount[0] != '\0') {
        wide_operand.hw = resolve_operand(shift_amount) / 16;
    } else {
        wide_operand.hw = 0;
    }

    dpr_instruction.rd = resolve_operand(rd);
    dpi_instruction.operand = generate_wide_move_operand(&wide_operand);
    dpr_instruction.data = generate_data_immediate_data(&dpi_instruction);
    return generate_data_processing_instruction(&dpr_instruction, false);
}

static uint32_t logical_instructions (char *opcode, char *rd, char *rn, char *rm, char *shift, char *shift_amount, bool register_64_bits) {
    data_processing_data_register data;
    data_processing_instruction dpr_instruction;
    dpr_instruction.sf = register_64_bits;
    dpr_instruction.rd = resolve_operand(rd);
    dpr_instruction.maybe_M = 0;
    bool n = false;
    if (strcmp(opcode, BIT_CLEAR) == 0 || strcmp(opcode, OR_NOT) == 0 || strcmp(opcode, EXCLUSIVE_OR_NOT) == 0 || strcmp(opcode, BIT_CLEAR_SET) == 0) {
        n = true;
    }
    if (strcmp(opcode, AND) == 0 || strcmp(opcode, BIT_CLEAR) == 0) {
        dpr_instruction.opc = 0b00;
    }
    else if (strcmp(opcode, OR) == 0 || strcmp(opcode, OR_NOT) == 0) {
        dpr_instruction.opc = 0b01;
    }
    else if (strcmp(opcode, EXCLUSIVE_OR) == 0 || strcmp(opcode, EXCLUSIVE_OR_NOT) == 0){
        dpr_instruction.opc = 0b10;
    }
    else {
        dpr_instruction.opc = 0b11;
    }
    if (shift[0] != '\0') {
        data.operand = resolve_operand(shift_amount);

        data.opr = 0;
        if (strncmp(shift, "lsl", 3) == 0) {
            data.opr = 0b0000;
        }
        else if (strncmp(shift, "lsr", 3) == 0) {
            data.opr = 0b0010;
        }
        else if (strncmp(shift, "asr", 3) == 0){
            data.opr = 0b0100;
        }
        else if (strncmp(shift, "ror", 3) == 0){
            data.opr = 0b0110;
        }
        if (n) {
            data.opr |= n;
        }
    }
    else {
        data.opr = n; // 0011 or 0010
        data.operand = 0;
    }
    data.rm = resolve_operand(rm);
    data.rn = resolve_operand(rn);
    dpr_instruction.data = generate_data_register_data(&data);
    return generate_data_processing_instruction(&dpr_instruction, true);
}




static uint32_t arithmetic_instructions (char *opcode, char *rd, char *rn, char *rm, char *shift, char* shift_amount, bool register_64_bits) {
    data_processing_data_register data;
    data_processing_instruction dpr_instruction;
    arithmetic_immediate_operand arith_operand;
    data_processing_data_immediate dpi_instruction;
    dpr_instruction.sf = register_64_bits;
    dpr_instruction.rd = resolve_operand(rd);
    if (strcmp(opcode, ADD) == 0) {
        dpr_instruction.opc = 0b00;
    }
    else if (strcmp(opcode, ADD_SIGNED) == 0) {
        dpr_instruction.opc = 0b01;
    }
    else if (strcmp(opcode, SUBTRACT) == 0){
        dpr_instruction.opc = 0b10;
    }
    else {
        dpr_instruction.opc = 0b11;
    }
    if (strncmp(rm, "#",1) == 0) {
        dpi_instruction.opi = 0b010;
        dpr_instruction.maybe_M = 1;
        arith_operand.imm12 = resolve_operand(rm);

        if (strcmp(shift, "lsl") == 0 && strcmp(shift_amount, "#12") == 0) {
            arith_operand.sh = 1;
        }
        else {
            arith_operand.sh = 0;
        }
        arith_operand.rn = resolve_operand(rn);
        dpi_instruction.operand = generate_arithmetic_operand(&arith_operand);
        dpr_instruction.data = generate_data_immediate_data(&dpi_instruction);
        return generate_data_processing_instruction(&dpr_instruction, false);
    }
    else {
        if (shift[0] != '\0') {
            data.operand = resolve_operand(shift_amount);
            if (strncmp(shift, "lsl", 3) == 0) {
                data.opr = 0b1000;
            }
            else if (strncmp(shift, "lsr", 3) == 0) {
                data.opr = 0b1010;
            }
            else if (strncmp(shift, "asr", 3) == 0){
                data.opr = 0b1100;
            }
            else {
                data.opr = 0b1110;
            }
        }
        else {
            data.opr = 0b1000;
            data.operand = 0;
        }
        dpr_instruction.maybe_M = 0;
        data.rm = resolve_operand(rm);
        data.rn = resolve_operand(rn);
        dpr_instruction.data = generate_data_register_data(&data);
        dpr_instruction.maybe_M = 0;
        return generate_data_processing_instruction(&dpr_instruction, true);
    }
}

static uint32_t multiply_instructions(char *mulopcode, char *rd, char *rn, char *rm, char *ra, bool register_64_bits) {
    data_processing_data_register data;
    data_processing_instruction dp_instruction;
    multiply_operand operand;
    if (strcmp(mulopcode, MULTIPLY_ADD) == 0 || strcmp(mulopcode, MULTIPLY) == 0) {
        madd(mulopcode, ra, &operand);
    }
    else if (strcmp(mulopcode, MULTIPLY_SUB) == 0 || !strcmp(mulopcode, MULTIPLY_NEG) == 0) {
        msub(mulopcode, ra, &operand);
    }
    data.operand = generate_operand(&operand);
    data.opr = 0b1000;
    data.rm = resolve_operand(rm);
    data.rn = resolve_operand(rn);
    dp_instruction.data = generate_data_register_data(&data);
    dp_instruction.maybe_M = 0b1;
    dp_instruction.opc = 0b00;
    dp_instruction.rd = resolve_operand(rd);
    dp_instruction.sf = register_64_bits;
    return generate_data_processing_instruction(&dp_instruction, 0b10);
}

uint32_t data_processing_assembly_init(instruction *instr) {
    // using find() to get the pointer to the first occurence
    bool register_64_bits = (strncmp(getString(instr->operands[0]),"x",1) == 0);
    // getting index from pointer
    int counter = 0;
    while (strcmp(data_processing_opcodes[counter], instr->opcode) != 0 && data_processing_opcodes[counter] != NULL) {
        counter++;
    };

    char *opc = instr->opcode;
    char *oper0 = getString(instr->operands[0]);
    char *oper1 = getString(instr->operands[1]);
    char *oper2 = getString(instr->operands[2]);
    char *oper3 = getString(instr->operands[3]);
    char *oper4 = getString(instr->operands[4]);
    
    int targetIndex = counter;

    if (targetIndex <= 3) {
        return arithmetic_instructions(opc, oper0, oper1, oper2, oper3, oper4, register_64_bits);
    }

    if (targetIndex == 4) {
        return arithmetic_instructions("subs", "rzr", oper0, oper1, oper2, oper3, register_64_bits);
    }

    if (targetIndex == 5) {
        return arithmetic_instructions("adds", "rzr", oper0, oper1, oper2, oper3, register_64_bits);
    }

    if (targetIndex == 6) {
        return arithmetic_instructions("sub", oper0, "rzr", oper1, oper2,oper3, register_64_bits);
    }

    if (targetIndex == 7) {
        return arithmetic_instructions("subs",  oper0, "rzr", oper1, oper2, oper3, register_64_bits);
    }

    if (targetIndex <= 15) {
        return logical_instructions(opc, oper0, oper1, oper2, oper3, oper4, register_64_bits);
    }

    if (targetIndex == 16) {
        return logical_instructions("ands", "rzr", oper0, oper1, oper2, oper3, register_64_bits);
    }

    if (targetIndex == 17) {
        return logical_instructions("orn", oper0,"rzr", oper1, oper2, oper3, register_64_bits);
    }

    if (targetIndex == 18) {
        return logical_instructions ("orr", oper0,"rzr", oper1, oper2, oper3, register_64_bits);
    }

    if (targetIndex <= 21) {
        return wide_move_instruction (opc, oper0, oper1, oper2, oper3, register_64_bits);
    }

    if (targetIndex <= 23) {
        return multiply_instructions(opc, oper0, oper1, oper2, oper3, register_64_bits);
    }

    if (targetIndex == 24) {
        return multiply_instructions("madd", oper0, oper1, oper2, "rzr", register_64_bits);
    }

    if (targetIndex == 25) {
        return multiply_instructions("msub", oper0, oper1, oper2, "rzr", register_64_bits);
    }

    fprintf(stderr, "data_processing: targetIndex not Found!");
    return 0;
}
