#ifndef ENCODE_INSTRUCTION_H
#define ENCODE_INSTRUCTION_H

#include <stdint.h>
#include "parser.h"

#define HALT_INSTRUCTION_BIG_ENDIAN 0x0000008A

uint32_t encode_instruction(instruction *instr);

#endif // ENCODE_INSTRUCTION_H
