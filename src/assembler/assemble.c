#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"
#include "parser.h"

#define LINE_LENGTH 256
#define STRING_LENGTH_64BIT 64 // 64 bit integer can be at most 20 characters long + null terminator
#define INITIAL_SYMBOL_TABLE_CAPACITY (size_t)64

//Function to convert 32 bit value to little endian
uint32_t to_little_endian(uint32_t value, uint8_t num_bytes) {
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
        instruction instr = parse(line, &address);
        if (strlen(instr.label) > 0) {
            // Add the symbol to the table along with its address
            add_symbol(table, instr.label, address);
        }
    }
    rewind(in); // Reset file pointer to beginning for the second pass
}

// Replace label in operand with its address from the symbol table
void replace_label_with_address(dynamicString* operand, uint64_t address) {
    char address_str[STRING_LENGTH_64BIT];
    sprintf(address_str, "%lu", address);
    freeDynamicString(*operand);
    *operand = createNewDynamicString(strlen(address_str) + 1);
    addString(*operand, address_str);
}

// Second pass: generate the binary output
void second_pass(FILE* in, FILE* out, const symbol_table* table) {
    char line[LINE_LENGTH];
    uint64_t address = 0;
    while(fgets(line, sizeof(line), in)) {
        instruction instr = parse(line, &address);
        for (int i = 0; i < 4; i++) {
            dynamicString operand = instr.operands[i];
            if (operand->current_size > 0) {
                uint64_t entry_address = find_entry(table, getString(operand));
                if (entry_address != UINT64_MAX) { // Check if entry exists
                    replace_label_with_address(&operand, entry_address);
                }
            }
        }
        uint32_t binary_instr = encode_instruction(&instr, table);
        binary_instr = to_little_endian(binary_instr, 4);
        fwrite(&binary_instr, sizeof(binary_instr),1 , out);
    }
}

int main(int argc, char **argv) { 
    //Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    //Open input file for reading
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return EXIT_FAILURE;
    }

    //Open output file for writing
    FILE* output_file = fopen(argv[2], "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Failed to open output file\n");
        fclose(input_file); // Close input file before returning
        return EXIT_FAILURE;
    }

    //Initalise symbol table with set initial capacity
    symbol_table* table = init_symbol_table(INITIAL_SYMBOL_TABLE_CAPACITY);

    //Perform first pass to build symbol table
    first_pass(input_file, table);

    //Perform second pass to generate binary output
    second_pass(input_file, output_file, table);

    //Free the symbol table and close the files
    free_symbol_table(table);
    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;

}
