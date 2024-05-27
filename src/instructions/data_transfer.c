#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#define bit_width_mask (1 << 31)
#define opc_mask (3 << 29)
#define opi_mask (7 << 23)
#define rd_mask 31
#define m_mask (1 << 28)
#define opr_mask (15 << 21)

#define arithI_mask
#define wide_move_mask

#define sh_mask (1 << 17)        // highest bit
#define imm12_mask (0xFFF << 5)  // 2^12-1, middle bits
#define rn_mask 31               // 2^5 - 1, lower 5 bits
extern struct CPU cpu;
void data_processing_init(int instruction, bool is_immediate, CPU *cpu) {
	bool is_64 = (bit_width_mask & instruction) >> 31 == 1;
	int opc = (instruction & opc_mask) >> 29;
	int rd = (instruction & rd_mask);
}

void data_processing_immediate(bool is_64, int opc, int opi, int rd, uint32_t instruction, CPU *cpu) {
		if (opi == 2) {
			arithmetic_immediate(is_64, opc, rd, , CPU);
		} else {
			wide_move(is_64, opc, rd, operand, CPU);
		}
}


void data_processing_register(bool is_64, int opc, int opr, int rd, uint32_t instruction, CPU *cpu) {
		// check if M = 1, then multiply
		if ((instruction & m_mask) >> 28 == 1) {
			multiply(is_64, rd, instruction, CPU);
		} else {
			// check if MSB of opr is 1, then arithmetic
			if ((instruction & opr_mask) >> 21 == 1) {
				arithmetic_register();
			} else {
				bit_logic();
			}
		}
}


// immediate instructions
void arithmetic_immediate(bool is_64, int opc, int rd, int operand, CPU *cpu) {
	bool left_shift = (instruction & sh_mask) >> 22 == 1;



}


void wide_move(bool is_64, int rd, int operand, CPU *cpu) {

}

// register instructions TODO
void arithmetic_register() {
}
