#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "symbol_table.h"

#define check_allocation(ptr) if (ptr == NULL) { fprintf(stderr, "An error occured allocating memory\n"); exit(1); }
#define resize_factor 2


symbol_table *init_symbol_table(size_t capacity) {
    symbol_table *new_table = malloc(sizeof(struct symbol_table));
    check_allocation(new_table);

    new_table->symbols = malloc(capacity * sizeof(struct symbol_entry));
    check_allocation(new_table->symbols);

    new_table->size = 0;
    new_table->capacity = capacity;

    return new_table;
}


static void resize_table(symbol_table* table, size_t capacity_scale_factor) {
    table->symbols = realloc(table->symbols, table->capacity * capacity_scale_factor);
    check_allocation(table->symbols);

    table->capacity *= capacity_scale_factor;
}


// automatically resizes if necessary
void add_entry(symbol_table *table , char *label, uint64_t address) {
    if (table->size >= table->capacity) {
        resize_table(table, resize_factor);
    }
    assert(table->size < table->capacity);

    symbol_entry *new_entry = malloc(sizeof(struct symbol_entry));
    new_entry->label = label;
    new_entry->address = address;
  
    table->symbols[table->size++] = new_entry;
}


uint64_t find_entry(const symbol_table* table, char *search_label) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp((table->symbols[i])->label, search_label)) {
            return (table->symbols[i])->address;
        }
    }
    return UINT64_MAX; // Special value indicating entry not found
}


void free_symbol_table(symbol_table *table) {
  free(table->symbols);
  free(table);
}

