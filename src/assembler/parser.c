// Assuming symbol table is defined as symbol_table
#include <ctype.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dynamicString.h"
#include "parser.h"

void free_instruction(instruction *instr, bool full_free) {
    freeDynamicString(instr->label);
    for (int i = 0; i < 4; i++) {
        freeDynamicString(instr->operands[i]);
    }
    if (full_free) {
        free(instr);
    }
}

void initialize_instruction(instruction *instr, bool free_previous) {
    if (free_previous) {
        free_instruction(instr, false);
    }

    instr->label = createNewDynamicString(10);
    memset(instr->opcode, 0, sizeof(instr->opcode));
    for (int i = 0; i < 4; i++) {
        instr->operands[i] = createNewDynamicString(10);
    }
    instr->complete = false;
    instr->line_number = 0;
}

void pretty_print(instruction *instr) {
    printf("Label: '%s'\n", getString(instr->label));
    printf("Opcode: '%s'\n", instr->opcode);
    printf("Operand 1: '%s'\n", getString(instr->operands[0]));
    printf("Operand 2: '%s'\n", getString(instr->operands[1]));
    printf("Operand 3: '%s'\n", getString(instr->operands[2]));
    printf("Operand 4: '%s'\n", getString(instr->operands[3]));
    printf("Complete: '%i'\n", instr->complete);
    printf("Line Number: '%lu'\n", instr->line_number);
    printf("\n");
}

static void compile_regex(regex_t *rx) {
    // Compile the regex
    int regex_compiled = regcomp(rx, "[A-Za-z].*:", 0);

    if (regex_compiled != 0) {
        fprintf(stderr, "Failed to compile REGEX");
        exit(1);
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
    } else if (new_instruction->complete) {
        // If the instruction is already complete then we want to start a new one
        initialize_instruction(new_instruction, true);
    }

    // Remove leading whitespace
    while (isspace(*current_line) != 0) {
        current_line++;
    }

    // Remove new line on end
    if (strlen(current_line) > 0 && current_line[strlen(current_line) - 1] == '\n') {
        current_line[strlen(current_line) - 1] = '\0';
    }

    // Check if empty line
    if (strlen(current_line) == 0) {
        return new_instruction;
    }

    regex_t rx;
    compile_regex(&rx);

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
    } else if (match == REG_NOMATCH) {
        // Instruction or empty line
        // Seperate by space and commas
        char *tok = strtok(current_line, ", ");
        // Keep track of which iteration it is
        int curr_it = 0;

        while (tok != NULL && curr_it < 5) {
            if (curr_it == 0) {
                strcpy(new_instruction->opcode, tok);
            } else {
                addString(new_instruction->operands[curr_it - 1], tok);
            }
            // printf("TOK %s %i\n", tok, curr_it);
            tok = strtok(NULL, ", ");
            curr_it++;
        }

        // Instruction is now complete and can be handled further
        new_instruction->complete = true;
        new_instruction->line_number = *current_line_counter;
        (*current_line_counter)++;

    } else {
        // Error occured???
        printf("ERROR OCCURED");
    }
    pretty_print(new_instruction);
    regfree(&rx);
    return new_instruction;
}