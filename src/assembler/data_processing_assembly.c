#include "data_processing_assembly.h"
#include "../emulator/data_processing.h"
#include "register_to_number.h"
#include <stdbool.h>

#define MULTIPLY_ADD "madd"
#define MULTIPLY_SUB "msub"
#define MULTIPLY "mul"
#define MULTIPLY_NEG "mneg"

void data_processing_init(instruction *instr) {
    char *data_processing_opcodes[] = { "add", "adds", "sub", "subs", "cmp", "cmn", "neg", "negs", "and", "ands", "bic", "bics", "eor", "eon", "orr", "orn", "tst", "mvn", "mov", "movn", "movk", "movz", "madd", "msub", "mul", "mneg", NULL };
    // using find() to get the pointer to the first
    // occurence
    int* targetPtr = find(&data_processing_opcodes[0], data_processing_opcodes + 26, instr->opcode);
    bool register_64_bits = (strncmp(&instr->operands[0],"x",1) == 0);
    // getting index from pointer
    int targetIndex = targetPtr - data_processing_opcodes;
    uint32_t result;
    switch (targetIndex)
    {
    case 0:
        
        break;
    case 1:
        
        break;
    case 2:
        
        break;
    case 3:
        
        break;
    case 4:
        
        break;
    case 5:
        
        break;
    case 6:
        
        break;
    case 7:
        
        break;
    case 8:
        
        break;
    case 9:
        
        break;
    case 10:
        
        break;
    case 11:
        
        break;
    case 12:
        
        break;
    case 13:
        
        break;
    case 14:
        
        break;
    case 15:
        
        break;
    case 16:
        
        break;
    case 17:
        
        break;
    case 18:
        
        break;
    case 19:
        
        break;
    case 20:
        
        break;
    case 21:
        
        break;
    case 22: // madd
        result = multiply_instructions(instr->opcode, &instr->operands[0], &instr->operands[1], &instr->operands[2], &instr->operands[3], register_64_bits);
        break;
    case 23: // msub
        result = multiply_instructions(instr->opcode, &instr->operands[0], &instr->operands[1], &instr->operands[2], &instr->operands[3], register_64_bits);
        break;
    case 24: // mul
        // fall through to mneg
    case 25: // mneg
        if (register_64_bits) {
            result = multiply_instructions(instr->opcode, &instr->operands[0], &instr->operands[1], &instr->operands[2], "xzr", register_64_bits);
        }
        else {
            result = multiply_instructions(instr->opcode, &instr->operands[0], &instr->operands[1], &instr->operands[2], "wzr", register_64_bits);
        }
        break;
    default:
        break;
    }
    return result;
}

uint32_t multiply_instructions(char *mulopcode, char *rd, char *rm, char *rn, char *ra, bool register_64_bits) {
    data_processing_data_register data;
    data_processing_instruction dp_instruction;
    multiply_operand operand;
    uint32_t result;
    if(strcmp(mulopcode, MULTIPLY_ADD) == 0 || strcmp(mulopcode, MULTIPLY) == 0) {
        madd(mulopcode, ra, &operand);
    }
    else if (strcmp(mulopcode, MULTIPLY_SUB) == 0 || strcmp(mulopcode, MULTIPLY_NEG) == 0) {
        msub(mulopcode, ra, &operand);
    }
    data.operand = generate_operand(&operand);
    data.opr = 0b1000;
    data.rm = register_to_number(rm);
    data.rn = register_to_number(rn);
    dp_instruction.data = generate_data_register(&data);
    dp_instruction.maybe_M = 0b1;
    dp_instruction.opc = 0b00;
    dp_instruction.rd = generate_data_register_data(&data);
    dp_instruction.sf = register_64_bits;
    result = generate_data_processing_instruction(dp_instruction);
    return result;
}

uint32_t generate_operand(multiply_operand *operand) {
    uint32_t result = 0;
    result = result | operand->x << 5;
    result = result | operand->ra;
    return result;
}

uint32_t generate_data_register_data(data_processing_data_register *data) {
    uint32_t result = 0;
    result = result | 0b1 << 20;
    result = result | data->opr << 16;
    result = result | (data->rm << 11);
    result = result | (data->operand << 5);
    result = result | data->rn;
    return result;
}

uint32_t generate_data_processing_instruction(data_processing_instruction instr) {
    uint32_t result = 0;
    result = result | instr.sf << 30;
    result = result | instr.opc << 28;
    result = result | instr.maybe_M << 27;
    result = result | (0b10) << 25;
    result = result | instr.data << 4;
    result = result | instr.rd;
    return result;
}

void madd(char *mulopcode, char *ra, multiply_operand *operand) {
    (*operand).x = 0;
    operand->ra = register_to_number(ra);
    return;
}

void msub(char *mulopcode, char *ra, multiply_operand *operand) {
    (*operand).x = 1;
    operand->ra = register_to_number(ra);
    return;
}