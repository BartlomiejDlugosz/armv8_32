#include <stdint.h>
#include "../structures.h"


union arithmetic_immediate_operand
{
	struct {
		unsigned sh : 1;
		unsigned imm12 : 12;
		unsigned rn : 5;
	};
	unsigned bits : 18;
};

union wide_move_operand
{
	struct {
		unsigned hw : 2;
		unsigned imm16 : 16;
	};
	unsigned bits : 18;
};

union single_data_processing_data_immediate {
    struct {
        unsigned operand: 18;
        unsigned opi: 3;
    };
    unsigned bits : 21;
};

union single_data_processing_data_register {
    struct {
        unsigned rn : 5;
        unsigned operand : 6;
        unsigned rm : 5;
        unsigned opr : 4;
        unsigned const_1: 1;
    };
    unsigned bits : 21;
};

// Defines union structure for the Single Data Processing instructions. Note that the data field in the struct varies for immediate and register based instructions, as defined above
union data_processing_instruction {
    struct {
        unsigned rd: 5;
        unsigned data : 21;
        unsigned : 2;
        unsigned maybe_M : 1;
        unsigned opc: 2;
        unsigned sf: 1;
    };
    uint32_t bits;
};
