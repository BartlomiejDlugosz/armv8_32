#include <stdint.h>

#include "structures.h"

union arithmetic_immediate_operand {
    struct {
        unsigned sh : 1;
        unsigned imm12 : 12;
        unsigned rn : 5;
    };
    unsigned bits : 18;
};

union wide_move_operand {
    struct {
        unsigned hw : 2;
        unsigned imm16 : 16;
    };
    unsigned bits : 18;
};

union data_processing_data_immediate {
    struct {
        unsigned operand : 18;
        unsigned opi : 3;
    };
    unsigned bits : 21;
};

union multiply_operand {
    struct {
        unsigned ra : 5;
        unsigned x : 1;
    };
    unsigned bits : 6;
};

union arithmetic_logic_opr {
    struct {
        unsigned type : 1;
        unsigned shift : 2;
        unsigned maybe_N : 1;
    };
    unsigned bits : 4;
};

union data_processing_data_register {
    struct {
        unsigned rn : 5;
        unsigned operand : 6;
        unsigned rm : 5;
        unsigned opr : 4;
        unsigned const_1 : 1;
    };
    unsigned bits : 21;
};

// Defines union structure for the Single Data Processing instructions. Note
// that the data field in the struct varies for immediate and register based
// instructions, as defined above
union data_processing_instruction {
    struct {
        unsigned rd : 5;
        unsigned data : 21;
        unsigned : 2;
        unsigned maybe_M : 1;
        unsigned opc : 2;
        unsigned sf : 1;
    };
    uint32_t bits;
};

// Handle immediate instructions
void perform_data_processing_immediate(
    CPU *cpu, union data_processing_instruction instr,
    union data_processing_data_immediate data);

uint64_t arithmetic_helper_64(CPU *cpu, unsigned opc, uint64_t rn_contents,
                              uint64_t op2);

uint32_t arithmetic_helper_32(CPU *cpu, unsigned opc, uint32_t rn_contents,
                              uint32_t op2);

void arithmetic_immediate_64(CPU *cpu, union data_processing_instruction instr,
                             union data_processing_data_immediate data,
                             union arithmetic_immediate_operand operand);

void arithmetic_immediate_32(CPU *cpu, union data_processing_instruction instr,
                             union data_processing_data_immediate data,
                             union arithmetic_immediate_operand operand);

void wide_move_64(CPU *cpu, union data_processing_instruction instr,
                  union data_processing_data_immediate data,
                  union wide_move_operand operand);

void wide_move_32(CPU *cpu, union data_processing_instruction instr,
                  union data_processing_data_immediate data,
                  union wide_move_operand operand);

// End of immediate instructions

// Handle register instructions

void perform_data_processing_register(CPU *cpu,
                                      union data_processing_instruction instr,
                                      union data_processing_data_register data);

void arithmetic_register_64(CPU *cpu, union data_processing_instruction instr,
                            union data_processing_data_register data,
                            union arithmetic_logic_opr opr);

void arithmetic_register_32(CPU *cpu, union data_processing_instruction instr,
                            union data_processing_data_register data,
                            union arithmetic_logic_opr opr);

void logic_64(CPU *cpu, union data_processing_instruction instr,
              union data_processing_data_register data,
              union arithmetic_logic_opr opr);

void logic_32(CPU *cpu, union data_processing_instruction instr,
              union data_processing_data_register data,
              union arithmetic_logic_opr opr);

void multiply_64(CPU *cpu, union data_processing_instruction instr,
                 union data_processing_data_register data,
                 union multiply_operand operand);

void multiply_32(CPU *cpu, union data_processing_instruction instr,
                 union data_processing_data_register data,
                 union multiply_operand operand);

// End of register instructions

void data_processing_init(CPU *cpu, uint32_t instruction, bool is_immediate) {
    union data_processing_instruction instr =
        (union data_processing_instruction){.bits = instruction};
    if (is_immediate) {
        union data_processing_data_immediate data =
            (union data_processing_data_immediate){.bits = instr.data};
        perform_data_processing_immediate(cpu, instr, data);
    } else {
        union data_processing_data_register data =
            (union data_processing_data_register){.bits = instr.data};
        perform_data_processing_register(cpu, instr, data);
    }
};
