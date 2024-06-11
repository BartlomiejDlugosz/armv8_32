

#include "includes.h"

typedef union {
    struct {
        unsigned rn : 5;
        unsigned imm12 : 12;
        unsigned sh : 1;
    };
    unsigned bits : 18;
} arithmetic_immediate_operand;

typedef union {
    struct {
        unsigned imm16 : 16;
        unsigned hw : 2;
    };
    unsigned bits : 18;
} wide_move_operand;

typedef union {
    struct {
        unsigned operand : 18;
        unsigned opi : 3;
    };
    unsigned bits : 21;
} data_processing_data_immediate;

typedef union {
    struct {
        unsigned ra : 5;
        unsigned x : 1;
    };
    unsigned bits : 6;
} multiply_operand;

typedef union {
    struct {
        unsigned maybe_N : 1;
        unsigned shift : 2;
        unsigned type : 1;
    };
    unsigned bits : 4;
} arithmetic_logic_opr;

typedef union  {
    struct {
        unsigned rn : 5;
        unsigned operand : 6;
        unsigned rm : 5;
        unsigned opr : 4;
        unsigned const_1 : 1;
    };
    unsigned bits : 21;
} data_processing_data_register;

// Defines typedef union structure for the Single Data Processing instructions. Note
// that the data field in the struct varies for immediate and register based
// instructions, as defined above
typedef union  {
    struct {
        unsigned rd : 5;
        unsigned data : 21;
        unsigned : 2;
        unsigned maybe_M : 1;
        unsigned opc : 2;
        unsigned sf : 1;
    };
    unsigned bits : 32;
} data_processing_instruction;

// Handle immediate instructions

uint64_t arithmetic_helper_64(CPU *cpu, unsigned opc, uint64_t rn_contents,
                              uint64_t op2);

uint32_t arithmetic_helper_32(CPU *cpu, unsigned opc, uint32_t rn_contents,
                              uint32_t op2);

void arithmetic_immediate_64(CPU *cpu, data_processing_instruction instr,
                             data_processing_data_immediate data,
                             arithmetic_immediate_operand operand);

void arithmetic_immediate_32(CPU *cpu, data_processing_instruction instr,
                             data_processing_data_immediate data,
                             arithmetic_immediate_operand operand);

void wide_move_64(CPU *cpu, data_processing_instruction instr,
                  data_processing_data_immediate data,
                  wide_move_operand operand);

void wide_move_32(CPU *cpu, data_processing_instruction instr,
                  data_processing_data_immediate data,
                  wide_move_operand operand);

// End of immediate instructions

// Handle register instructions

void arithmetic_register_64(CPU *cpu, data_processing_instruction instr,
                            data_processing_data_register data,
                            arithmetic_logic_opr opr);

void arithmetic_register_32(CPU *cpu, data_processing_instruction instr,
                            data_processing_data_register data,
                            arithmetic_logic_opr opr);

void logic_64(CPU *cpu, data_processing_instruction instr,
              data_processing_data_register data,
              arithmetic_logic_opr opr);

void logic_32(CPU *cpu, data_processing_instruction instr,
              data_processing_data_register data,
              arithmetic_logic_opr opr);

void multiply_64(CPU *cpu, data_processing_instruction instr,
                 data_processing_data_register data,
                 multiply_operand operand);

void multiply_32(CPU *cpu, data_processing_instruction instr,
                 data_processing_data_register data,
                 multiply_operand operand);

// End of register instructions
void perform_data_processing_immediate(
    CPU *cpu, data_processing_instruction instr,
    data_processing_data_immediate data);

void perform_data_processing_register(CPU *cpu,
                                      data_processing_instruction instr,
                                      data_processing_data_register data);

void data_processing_init(CPU *cpu, uint32_t instruction, bool is_immediate);
<<<<<<< Updated upstream

#endif  // DATA_PROCESSING_H
=======
>>>>>>> Stashed changes
