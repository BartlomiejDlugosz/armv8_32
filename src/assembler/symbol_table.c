#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "symbol_table.h"
#include "dynamicString.h"

#define CHECK_ALLOCATION(ptr) if (ptr == NULL) { fprintf(stderr, "An error occured allocating memory\n"); exit(1); }
#define RESIZE_FACTOR 2


symbol_table *init_symbol_table(size_t capacity) {
    symbol_table *new_table = malloc(sizeof(struct symbol_table));
    CHECK_ALLOCATION(new_table);

    new_table->symbols = malloc(capacity * sizeof(struct symbol_entry));
    CHECK_ALLOCATION(new_table->symbols);

    new_table->size = 0;
    new_table->capacity = capacity;

    return new_table;
}


static void resize_table(symbol_table* table, size_t capacity_scale_factor) {
    table->symbols = realloc(table->symbols, table->capacity * capacity_scale_factor);
    CHECK_ALLOCATION(table->symbols);

    table->capacity *= capacity_scale_factor;
}


// automatically resizes if necessary
void add_entry(symbol_table *table , dynamicString *label, uint64_t address) {
    if (table->size >= table->capacity) {
        resize_table(table, RESIZE_FACTOR);
    }
    assert(table->size < table->capacity);

    symbol_entry *new_entry = malloc(sizeof(struct symbol_entry));
    new_entry->label = label;
    new_entry->address = address;
  
    table->symbols[table->size++] = new_entry;
}


uint64_t find_entry(const symbol_table* table, dynamicString *search_label) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(getString(table->symbols[i]->label), getString(search_label)) == 0) {
            return table->symbols[i]->address;
        }
    }
    return UINT64_MAX; // Special value indicating entry not found
}


void free_symbol_table(symbol_table *table) {
    for (int i = 0; i < table->size; i++) {
        freeDynamicString(table->symbols[i]->label);
    }
    free(table->symbols);
    free(table);
}

