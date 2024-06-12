#ifndef PASSING_H
#define PASSING_H

#include <stdio.h>
#include "symbol_table.h"

#define LINE_LENGTH 256
#define STRING_LENGTH_64BIT 64 // 64 bit integer can be at most 20 characters long + null terminator
#define INITIAL_SYMBOL_TABLE_CAPACITY (size_t)64

// First pass: parse the source file and build the symbol table
void first_pass(FILE* in, symbol_table* table);
// Second pass: generate the binary output
void second_pass(FILE* in, FILE* out, const symbol_table* table);
#endif // PASSING_H