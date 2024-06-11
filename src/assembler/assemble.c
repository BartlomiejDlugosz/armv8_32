#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"

void first_pass(FILE* in, SymbolTable* table) {
    char line[256];
    uint32_t address = 0;
    while (fgets(line, sizeof(line), in)) {
        instruction instr = parse(line);
        if (strlen(instr.label) > 0) {
            add_symbol(table, instr.label, address);
        }
    }
}

int main(int argc, char **argv) { 
    return EXIT_SUCCESS; 
}
