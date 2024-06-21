#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dynamic_string.h"

dynamic_string *createNewDynamicString(int starting_number_of_elements) {
    dynamic_string *new_dynamic_string = malloc(sizeof(dynamic_string));
    if (new_dynamic_string == NULL) {
        fprintf(stderr, "An error occured allocating memory");
        exit(1);
    }
    memset(new_dynamic_string, 0, sizeof(dynamic_string));

    new_dynamic_string->data =
        malloc(starting_number_of_elements * sizeof(char));
    if (new_dynamic_string->data == NULL) {
        fprintf(stderr, "An error occured allocating memory");
        exit(1);
    }
    memset(new_dynamic_string->data, 0,
           starting_number_of_elements * sizeof(char));

    new_dynamic_string->max_size = starting_number_of_elements;

    return new_dynamic_string;
}

void resizeArray(dynamic_string *arr) {
    arr->data = realloc(arr->data, arr->max_size * 2);
    if (arr->data == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        exit(1);
    }
    memset(arr->data + arr->max_size, 0, arr->max_size);

    arr->max_size *= 2;
    printf("Resized to %i\n", arr->max_size);
}

void addChar(dynamic_string *arr, char element) {
    if (arr->current_size + 1 >= arr->max_size) {
        // reallocate more memory
        resizeArray(arr);
    }
    arr->data[arr->current_size++] = element;
}

void addString(dynamic_string *arr, char *str) {
    while (*str != '\0') {
        addChar(arr, *str);
        str++;
    }
}

char getChar(dynamic_string *arr, int index) { return arr->data[index]; }

char *getString(dynamic_string *arr) { return arr->data; }

void freeDynamicString(dynamic_string *arr) {
    free(arr->data);
    free(arr);
}
