

#include "includes.h"

typedef struct {
        unsigned rn : 5;
        unsigned imm12 : 12;
        unsigned sh : 1;
} arithmetic_immediate_operand;

typedef struct {
        unsigned imm16 : 16;
        unsigned hw : 2;
} wide_move_operand;

typedef struct {
        unsigned operand : 18;
        unsigned opi : 3;
} data_processing_data_immediate;

typedef struct {
        unsigned ra : 5;
        unsigned x : 1;
} multiply_operand;

typedef struct {
        unsigned maybe_N : 1;
        unsigned shift : 2;
        unsigned type : 1;
} arithmetic_logic_opr;

typedef struct {
        unsigned rn : 5;
        unsigned operand : 6;
        unsigned rm : 5;
        unsigned opr : 4;
        //unsigned const_1 : 1; // not needed as not using union
} data_processing_data_register;

// Defines typedef union structure for the Single Data Processing instructions. Note
// that the data field in the struct varies for immediate and register based
// instructions, as defined above
typedef struct  {
        unsigned rd : 5;
        unsigned data : 21;
        //unsigned : 2; // not needed as not using union
        unsigned maybe_M : 1;
        unsigned opc : 2;
        unsigned sf : 1;
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
              data_processing_data_register data, arithmetic_logic_opr opr);

void logic_32(CPU *cpu, data_processing_instruction instr,
              data_processing_data_register data, arithmetic_logic_opr opr);

void multiply_64(CPU *cpu, data_processing_instruction instr,
                 data_processing_data_register data, multiply_operand operand);

void multiply_32(CPU *cpu, data_processing_instruction instr,
                 data_processing_data_register data, multiply_operand operand);

// End of register instructions
void perform_data_processing_immediate(CPU *cpu,
                                       data_processing_instruction instr,
                                       data_processing_data_immediate data);

void perform_data_processing_register(CPU *cpu,
                                      data_processing_instruction instr,
                                      data_processing_data_register data);

void data_processing_init(CPU *cpu, uint32_t instruction, bool is_immediate);


arithmetic_immediate_operand init_arithmetic_immediate_operand(uint32_t operand);


wide_move_operand init_wide_move_operand(uint32_t operand);


data_processing_data_immediate init_data_processing_data_immediate(uint32_t data);


multiply_operand init_multiply_operand(uint32_t operand);


arithmetic_logic_opr init_arithmetic_logic_opr(uint32_t opr);


data_processing_data_register init_data_processing_data_register(uint32_t data);


data_processing_instruction init_data_processing_instruction(uint32_t instruction);
 

#endif  // DATA_PROCESSING_H
