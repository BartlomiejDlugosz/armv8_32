#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "symbol_table.h"

#define check_allocation(ptr) if (ptr == NULL) { fprintf(stderr, "An error occured allocating memory\n"); exit(1); }
#define resize_factor 2


symbol_table *init_symbol_table(size_t initial_size, size_t capacity) {
  symbol_table *new_table = malloc(sizeof(struct symbol_table));
  check_allocation(new_table);

  new_table->symbols = malloc(capacity * sizeof(symbol_entry));
  check_allocation(new_table->symbols);

  new_table->size = initial_size;
  new_table->capacity = capacity;

  return new_table;
}


// automatically resizes if necessary
// returns a bool to indicate whether the 
// entry was successfully added
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


uint64_t find_entry(symbol_table* table, char *search_label) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp((table->symbols[i])->label, search_label)) {
            return (table->symbols[i])->address;
        }
    }
    printf("tried to find entry '%s' but failed\n", search_label);
    exit(1);
}


void free_symbol_table(symbol_table *table) {

  free(table->symbols);
  free(table);
}


static void resize_table(symbol_table* table, size_t capacity_scale_factor) {
    table->symbols = realloc(table->symbols, table->capacity * capacity_scale_factor);
    check_allocation(table->symbols);

    table->capacity *= capacity_scale_factor;
}

