#ifndef SINGLE_DATA_TRANSFER_ASSEMBLY_H
#define SINGLE_DATA_TRANSFER_ASSEMBLY_H

#include <stdint.h>
#include "parser.h"

// Function prototype for converting single data transfer instruction to binary
uint32_t single_data_transfer_to_binary(instruction* instr);

#endif  // SINGLE_DATA_TRANSFER_ASSEMBLY_H
