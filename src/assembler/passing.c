#include <stdlib.h>
#include <string.h>
#include "passing.h"
#include "parser.h"


//Function to convert 32 bit value to little endian
static uint32_t to_little_endian(uint32_t value, uint8_t num_bytes) {
    uint32_t result = 0;
    for (uint8_t i = 0; i < num_bytes; i++) {
        result |= ((value >> (8 * i)) & 0xFF) << (8 * i);
    }
    return result;
}

// First pass: parse the source file and build the symbol table
void first_pass(FILE* in, symbol_table* table) {
    char line[LINE_LENGTH];
    uint64_t address = 0;
    //Address to be incremented by parser
    while (fgets(line, sizeof(line), in)) {
        instruction* instr = parse(line, &address);
        if (instr->complete && strlen(getString(instr->label)) > 0) { 
            // Add the symbol to the table along with its address
            add_entry(table, getString(instr->label), address);
        }
    }
    rewind(in); // Reset file pointer to beginning for the second pass
}

// Replace label in operand with its address from the symbol table
static void replace_label_with_address(instruction *instr, int index, uint64_t address) {
    char address_str[STRING_LENGTH_64BIT];
    sprintf(address_str, "%lu", address);
    freeDynamicString(instr->operands[index]);
    instr->operands[index] = createNewDynamicString(strlen(address_str) + 1);
    addString(instr->operands[index], address_str);
}

// Second pass: generate the binary output
void second_pass(FILE* in, FILE* out, const symbol_table* table) {
    char line[LINE_LENGTH];
    uint64_t address = 0;
    instruction* instr;
    while(fgets(line, sizeof(line), in)) {
        instr = parse(line, &address);
        for (int i = 0; i < 4; i++) {
            dynamicString *operand = instr->operands[i];
            if (operand->current_size > 0) {
                uint64_t entry_address = find_entry(table, getString(operand));
                if (entry_address != UINT64_MAX) { // Check if entry exists
                    replace_label_with_address(instr, i, entry_address);
                }
            }
        }
        uint32_t binary_instr; // = encode_instruction(&instr, table); Implement this function
        binary_instr = to_little_endian(binary_instr, 4);
        fwrite(&binary_instr, sizeof(binary_instr),1 , out);
    }
    free_instruction(instr, true);
}
