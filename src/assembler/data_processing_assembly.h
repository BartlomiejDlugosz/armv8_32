#ifndef DATA_PROCESSING_ASSEMBLY_H
#define DATA_PROCESSING_ASSEMBLY_H
#include "parser.h"
#include <stdint.h>
void data_processing_init(instruction *instr);
uint32_t multiply_instructions(char *mulopcode, char *rd, char *rm, char *rn, char *ra, bool register_64_bits);
uint32_t generate_operand(multiply_operand *operand);
uint32_t generate_data_register_data(data_processing_data_register *data);
uint32_t generate_data_processing_instruction(data_processing_instruction instr);
void madd(char *mulopcode, char *ra, multiply_operand *operand);
void msub(char *mulopcode, char *ra, multiply_operand *operand);
#endif // DATA_PROCESSING_ASSEMBLY_H