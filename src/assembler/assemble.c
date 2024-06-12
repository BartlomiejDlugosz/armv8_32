#include "passing.h"

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

    //Initalise symbol table with set initial capacity
    symbol_table* table = init_symbol_table(INITIAL_SYMBOL_TABLE_CAPACITY);

    //Perform first pass to build symbol table
    first_pass(input_file, table);

    
    //Open output file for writing
    FILE* output_file = fopen(argv[2], "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Failed to open output file\n");
        fclose(input_file); // Close input file before returning
        return EXIT_FAILURE;
    }

    //Perform second pass to generate binary output
    second_pass(input_file, output_file, table);

    //Free the symbol table and close the files
    free_symbol_table(table);
    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;

}
