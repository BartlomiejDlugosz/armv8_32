#pragma once

#include <stdio.h>

#define PRINT_INVALID_REGISTER_INDEX(reg_index) \
    fprintf(stderr, "Invalid register index: %u\n", reg_index)

#define PRINT_INVALID_MEMORY_ACCESS(address) \
    fprintf(stderr, "Invalid memory access at address: 0x%08x\n", address)

