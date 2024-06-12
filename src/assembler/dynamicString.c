#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dynamicString.h"

dynamicString createNewDynamicString(int starting_number_of_elements) {
    dynamicString new_dynamic_string = malloc(sizeof(struct dynamicString));
    if (new_dynamic_string == NULL) {
        fprintf(stderr, "An error occured allocating memory");
        exit(1);
    }
    memset(new_dynamic_string, 0, sizeof(struct dynamicString));

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

void resizeArray(dynamicString arr) {
    arr->data = realloc(arr->data, arr->max_size * 2);
    if (arr->data == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        exit(1);
    }
    memset(arr->data + arr->max_size, 0, arr->max_size);

    arr->max_size *= 2;
    printf("Resized to %i\n", arr->max_size);
}

void addChar(dynamicString arr, char element) {
    if (arr->current_size + 1 >= arr->max_size) {
        // reallocate more memory
        resizeArray(arr);
    }
    arr->data[arr->current_size++] = element;
}

void addString(dynamicString arr, char *str) {
    while (*str != '\0') {
        addChar(arr, *str);
        str++;
    }
}

char getChar(dynamicString arr, int index) { return arr->data[index]; }

char *getString(dynamicString arr) { return arr->data; }

void freeDynamicString(dynamicString arr) {
    free(arr->data);
    free(arr);
}
