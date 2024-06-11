#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"
#include "instruction_parser.h"

#define LINE_LENGTH 256

void first_pass(FILE* in, SymbolTable* table) {
    char line[LINE_LENGTH];
    uint32_t address = 0;
    while (fgets(line, sizeof(line), in)) {
        instruction instr = parse(line);
        if (strlen(instr.label) > 0) {
            add_symbol(table, instr.label, address);
        }

        // Need to update the address based on the instruction size
        // Assuming each instruction is 4 bytes for simplicity
        address += 4;
    }

    rewind(in); // Reset file pointer to beginning for the second pass
}

void second_pass(FILE* in, FILE* out, const SymbolTable* table) {
    char line[LINE_LENGTH];
    while(fgets(line, sizeof(line), in)) {
        instruction instr = parse(line);
        uint32_t binary_instr = encode_instruction(&instr, table); 
    }
}

int main(int argc, char **argv) { 
    return EXIT_SUCCESS; 
}
