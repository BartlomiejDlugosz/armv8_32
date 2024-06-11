

#include "includes.h"

#define INVALID_REGISTER_INDEX(reg_index)                       \
    fprintf(stderr, "Invalid register index: %u\n", reg_index); \
    exit(EXIT_FAILURE);

#define INVALID_MEMORY_ACCESS(address)                                      \
    fprintf(stderr, "Invalid memory access at address: 0x%08x\n", address); \
    exit(EXIT_FAILURE);

#define OPC_CASE_ERROR()                                                   \
    fprintf(stderr,                                                        \
            "Invalid operation code (opc) encountered during instruction " \
            "execution.\n");                                               \
    exit(EXIT_FAILURE);

#define SHIFT_TYPE_DETERMINATION_ERROR()                                     \
    fprintf(stderr,                                                          \
            "Invalid or unsupported shift type encountered for operand op2 " \
            "during instruction execution.\n");                              \
    exit(EXIT_FAILURE);

#define UNDEFINED_INSTRUCTION()                 \
    fprintf(stderr, "Undefined Instruction\n"); \
    exit(EXIT_FAILURE);

#define INVALID_XZR_ENCODING()                                         \
    fprintf(stderr, "Invalid XZR encoding for branch instruction.\n"); \
    exit(EXIT_FAILURE);
