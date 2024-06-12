// Assuming symbol table is defined as symbol_table
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dynamicString.h"
#include "parser.h"

void initialize_instruction(instruction *instr, bool free_previous) {
    if (free_previous) {
        freeDynamicString(instr->label);
        for (int i = 0; i < 4; i++) {
            freeDynamicString(instr->operands[i]);
        }
    }

    instr->label = createNewDynamicString(10);
    memset(instr->opcode, 0, sizeof(instr->opcode));
    for (int i = 0; i < 4; i++) {
        instr->operands[i] = createNewDynamicString(10);
    }
}

instruction *parse(char *current_line, uint64_t *current_line_counter) {
    // static so it can be saved for following calls
    static bool initialized = false;
    static instruction *new_instruction = NULL;
    // Check if this is the first time calling function and initialize the
    // new instruction
    if (!initialized) {
        initialized = true;
        new_instruction = malloc(sizeof(instruction));
        if (new_instruction == NULL) {
            fprintf(stderr, "Error occured when allocating memory");
            exit(1);
        }
        initialize_instruction(new_instruction, false);
    }
    // Regex expression for determining if label
    regex_t rx;
    int regex_compiled;

    // Compile the regex
    regex_compiled = regcomp(&rx, "[A-Za-z].*:", 0);
    if (regex_compiled != 0) {
        fprintf(stderr, "Failed to compile REGEX");
        exit(1);
    }

    // Remove leading whitespace
    while (isspace(*current_line) != 0) {
        current_line++;
    }

    // Try to match the line with the regex
    int match = regexec(&rx, current_line, 0, NULL, 0);
    if (match == 0) {
        // Matches REGEX for a label
        // Reinitialise new instruction
        initialize_instruction(new_instruction, true);

        // Read in label into dynamic string
        while (*current_line != ':') {
            addChar(new_instruction->label, *current_line);
            current_line++;
        }
        // Set the label and mark the instruction as not yet complete, since
        // opcode and operands need to be added in the next run
        new_instruction->complete = false;
    } else if (match == REG_NOMATCH) {
        // Instruction or empty line
        char *tok = strtok(current_line, ", ");

        while (tok != NULL) {
            printf(" %s\n", tok);

            tok = strtok(NULL, ", ");
        }

        // Instruction is now complete and can be handled further
        new_instruction->complete = true;
    } else {
        // Error occured???
        printf("ERROR OCCURED");
    }
    regfree(&rx);
    return new_instruction;
}
