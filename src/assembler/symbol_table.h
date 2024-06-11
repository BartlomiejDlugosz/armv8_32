#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct symbol_entry {
    char *label;
    uint64_t address;
} symbol_entry;

typedef struct symbol_table {
    symbol_entry **symbols;
    size_t size;
    size_t capacity;
} symbol_table;


symbol_table *init_symbol_table(size_t capacity);
void add_entry(symbol_table *table , char *label, uint64_t address); // automatically resizes if necessary
uint64_t find_entry(symbol_table *table, char *search_label);
void free_symbol_table(symbol_table *table); 

static void resize_table(symbol_table* table, size_t capacity_scale_factor);

#endif // SYMBOL_TABLE_H
