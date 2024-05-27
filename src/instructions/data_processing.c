#include <stdio.h>
#include <stdbool.h>
#include <stdin.h>
#include <data_processing.h>
#include <limits.h> // for INT_MAX, although I should probably hardcode the values for the architecture

// Handle immediate instructions
void perform_data_processing_immediate(
		CPU *cpu, 
		union data_processing_instruction instr,
		union data_processing_data_immediate data
) {
	if (opi == 2) {
		union arithmetic_immediate_operand operand = (union arithmetic_immediate_operand) {.bits = instruction.operand};
		if (instr.sh == 1) {
			arithmetic_immediate_64();
		} else {
			arithmetic_immediate_32();
		}
	} else if (opi == 5) {
		union wide_move_operand operand = (union wide_move_operand operand) {.bits = instruction.operand};
		if (instr.sh == 1) {
			wide_move_64();
		} else {
			wide_move_32();
		}
	} else {
		printf("undefined instruction");
	}
};


void arithmetic_immediate_64(
		CPU *cpu, 
		union data_processing_instruction instr,
		union data_processing_data_immediate data,
		union arithmetic_immediate_operand operand
) {
	// This function performs arithmetic with 64-bit registers
	uint64_t op2 = (uint64_t) (operand.sh == 1 ? operand.imm12 << 12 : operand.imm12);

	if (rn == 0b11111) {
		// rn
		printf("error: reached a case we are not handling!");
		printf("error: rn is stack pointer!");
		return;
	}
	uint64_t rn_contents = read_register64(*cpu, operand.rn);

	// Checking for unsigned overflow
	// from https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
	//if (x > 0 && a > INT_MAX - x) // `a + x` would overflow (1)
	//if (x < 0 && a < INT_MIN - x) // `a + x` would underflow (2)
	//if (x < 0 && a > INT_MAX + x) // `a - x` would overflow (3)
	//if (x > 0 && a < INT_MIN + x) // `a - x` would underflow (4)
	// the check for x > 0 (or in our case op2 > 0) is ommitted
	// since this is given. This also means that case 
	// (2) and (3) above are not possible.
	// Hence, addition cannot underflow and subtraction cannot overflow
	//
	// we need to adapt the max/min values for 64-bit
	bool add_overflow = (operand.rn > LONG_MAX - op2)   // `rn + op2` will overflow (signed)
	bool sub_underflow = (operand.rn < LONG_MIN + op2)   // `rn - op2` will underflow (signed)
	bool uadd_overflow = (operand.rn > ULONG_MAX - op2) // `rn + op2` will produce a carry (unsigned)
	bool usub_underflow = (operand.rn < op2)             // `rn - op2` will produce a borrow (unsigned)
	// this might be dangerous because the values are platform dependent. maybe hardcode for architecture?

	uint64_t result;
	switch (instr.opc) {
		case 0b00: // add
			if (instr.rd) == 0b11111
			result = rn_contents + op2;
			break;
		case 0b01: // adds
			result = rn_contents + op2;
			*cpu.PSTATE.N = (uint8_t) (result >> 63) // extract MSB (sign bit)
			*cpu.PSTATE.Z = (uint8_t) (result == 0 ? 1 : 0);
			*cpu.PSTATE.C = (uint8_t) (uadd_overflow ? 1 : 0);
			*cpu.PSTATE.V = (uint8_t) (add_overflow ? 1 : 0);
			break;
		case 0b10: // sub
			result = operand.rn - op2;
			break;
		case 0b11: // subs
			result = operand.rn - op2;
			*cpu.PSTATE.N = (uint8_t) (result >> 63) // extract MSB (sign bit)
			*cpu.PSTATE.Z = (uint8_t) (result == 0 ? 1 : 0);
			*cpu.PSTATE.C = (uint8_t) (usub_overflow ? 1 : 0);
			*cpu.PSTATE.V = (uint8_t) (sub_overflow ? 1 : 0);
			break;
		default:
			printf("Something has gone wrong in opc case!");
	}

	if (instr.rd == 0b11111 && (instr.opc == 0b00 || instr.opc == ob10)) {
		// rd encodes the stack pointer, not handled
		printf("error: reached a case we are not handling!");
		printf("error: rn is stack pointer!");
		return;
	};

	write_register64(*cpu, instr.rd, result);
	return;
};


void arithmetic_immediate_32(
		CPU *cpu, 
		union data_processing_instruction instr,
		union data_processing_data_immediate data,
		union arithmetic_immediate_operand operand
) {
	// This function performs arithmetic with 64-bit registers
	uint32_t op2 = (uint32_t) (operand.sh == 1 ? operand.imm12 << 12 : operand.imm12);

	if (rn == 0b11111) {
		// rn
		printf("error: reached a case we are not handling!");
		printf("error: rn is stack pointer!");
		return;
	}
	uint64_t rn_contents = read_register32(*cpu, operand.rn);

	// Checking for unsigned overflow
	// from https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
	//if (x > 0 && a > INT_MAX - x) // `a + x` would overflow (1)
	//if (x < 0 && a < INT_MIN - x) // `a + x` would underflow (2)
	//if (x < 0 && a > INT_MAX + x) // `a - x` would overflow (3)
	//if (x > 0 && a < INT_MIN + x) // `a - x` would underflow (4)
	// the check for x > 0 (or in our case op2 > 0) is ommitted
	// since this is given. This also means that case 
	// (2) and (3) above are not possible.
	// Hence, addition cannot underflow and subtraction cannot overflow
	//
	// we need to adapt the max/min values for 64-bit
	bool add_overflow = (operand.rn > INT_MAX - op2)   // `rn + op2` will overflow (signed)
	bool sub_underflow = (operand.rn < INT_MIN + op2)   // `rn - op2` will underflow (signed)
	bool uadd_overflow = (operand.rn > UINT_MAX - op2) // `rn + op2` will produce a carry (unsigned)
	bool usub_underflow = (operand.rn < op2)             // `rn - op2` will produce a borrow (unsigned)
	// this might be dangerous because the values are platform dependent. maybe hardcode for architecture?

	uint64_t result;
	switch (instr.opc) {
		case 0b00: // add
			if (instr.rd) == 0b11111
			result = rn_contents + op2;
			break;
		case 0b01: // adds
			result = rn_contents + op2;
			*cpu.PSTATE.N = (uint8_t) (result >> 31) // extract MSB (sign bit)
			*cpu.PSTATE.Z = (uint8_t) (result == 0 ? 1 : 0);
			*cpu.PSTATE.C = (uint8_t) (uadd_overflow ? 1 : 0);
			*cpu.PSTATE.V = (uint8_t) (add_overflow ? 1 : 0);
			break;
		case 0b10: // sub
			result = operand.rn - op2;
			break;
		case 0b11: // subs
			result = operand.rn - op2;
			*cpu.PSTATE.N = (uint8_t) (result >> 31) // extract MSB (sign bit)
			*cpu.PSTATE.Z = (uint8_t) (result == 0 ? 1 : 0);
			*cpu.PSTATE.C = (uint8_t) (usub_overflow ? 1 : 0);
			*cpu.PSTATE.V = (uint8_t) (sub_overflow ? 1 : 0);
			break;
		default:
			printf("Something has gone wrong in opc case!");
	}

	if (instr.rd == 0b11111 && (instr.opc == 0b00 || instr.opc == ob10)) {
		// rd encodes the stack pointer, not handled
		printf("error: reached a case we are not handling!");
		printf("error: rn is stack pointer!");
		return;
	};

	write_register32(*cpu, instr.rd, result);
	return;
};



void wide_move_64(
		CPU *cpu, 
		union data_processing_instruction instr,
		union data_processing_data_immediate data,
		union wide_move_operand operand
); {
		uint64_t shift = operand.hw * 16;
		uint64_t op = operand.imm16 << shift;
		uint64_t result;
		uint64_t current_rd = read_register64(*cpu, instr.rd);

		switch (instr.opc) {
			case 0b00:
				result = ~op;
				break;
			//case 0b01:
			//	printf("something went wrong in opc case!"); fall through to default
			case 0b10:
				result = op;
				break;
			case 0b11:
				// keep_mask: 16 bits set to one where imm16 is to be inserted
				uint64_t keep_mask = 0xFFFF << shift;
				current_rd = current_rd && (~keep_mask); // clear bits
				result = current_rd || op; // set bits using shifted imm16
				break;
			default:
				printf("something went wrong in opc case!");
				break;

		}

		write_register64(*cpu, instr.rd, result);
		return;
}


void wide_move_32(
		CPU *cpu, 
		union data_processing_instruction instr,
		union data_processing_data_immediate data,
		union wide_move_operand operand
); {
		if (operand.hw > 0b01) {
			// shift is too large for 32-bit register
			printf("error: shift is too large for register size!");
			return;
		}

		uint32_t shift = operand.hw * 16; // pre: hw < 2
		uint32_t op = operand.imm16 << shift;
		uint32_t result;
		uint32_t current_rd = read_register32(*cpu, instr.rd);

		switch (instr.opc) {
			case 0b00:
				result = ~op;
				break;
			//case 0b01:
			//	printf("something went wrong in opc case!"); fall through to default
			case 0b10:
				result = op;
				break;
			case 0b11:
				// keep_mask: 16 bits set to one where imm16 is to be inserted
				uint32_t keep_mask = 0xFFFF << shift;
				current_rd = current_rd && (~keep_mask); // clear bits
				result = current_rd || op; // set bits using shifted imm16
				break;
			default:
				printf("something went wrong in opc case!");
				break;

		}

		write_register32(*cpu, instr.rd, result);
		return;
}

// End of immediate instructions

// Handle register instructions


void perform_data_processing_register() {

};



// End of register instructions


void data_processing_init(CPU *cpu, uint32_t instruction, bool is_immediate) {
    union data_processing_instruction instr = (union single_data_transfer_instruction) {.bits = instruction};

    if (is_immediate) {
	    union data_processing_data_immediate data = (union data_processing_data_immediate) {.bits = instr.data};
	    perform_data_processing_immediate();

    } else {
	    union data_processing_data_register data = (union data_processing_data_register) {.bits = instr.data};
	    perform_data_processing_register();
    }

};
 
