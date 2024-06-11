#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char* label;
    uint64_t address;
} Symbol;

typedef struct 
{
    Symbol* symbols;
    size_t size;
    size_t capacity;
} SymbolTable;

SymbolTable* initalise_symbol_table();
void free_symbol_table(SymbolTable* table);
void add_symbol(SymbolTable *table, const char *label, uint64_t address);
int find_symbol(const SymbolTable* table, const char* label, uint64_t* address);

#endif // SYMBOL_TABLE_H
