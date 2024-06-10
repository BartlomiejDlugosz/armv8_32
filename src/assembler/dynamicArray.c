#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dynamicArray.h"

dynamicArray createNewDynamicArray(int starting_number_of_elements) {
  dynamicArray new_dynamic_array = malloc(sizeof(struct dynamicArray));
  if (new_dynamic_array == NULL) {
    fprintf(stderr, "An error occured allocating memory");
    exit(1);
  }
  memset(new_dynamic_array, 0, sizeof(struct dynamicArray));

  new_dynamic_array->data = malloc(starting_number_of_elements * sizeof(char));
  if (new_dynamic_array->data == NULL) {
    fprintf(stderr, "An error occured allocating memory");
    exit(1);
  }
  memset(new_dynamic_array->data, 0, starting_number_of_elements * sizeof(char));

  new_dynamic_array->max_size = starting_number_of_elements;

  return new_dynamic_array;
}

void resizeArray(dynamicArray arr) {
  arr->data = realloc(arr->data, arr->max_size * 2);
  if (arr->data == NULL) {
    fprintf(stderr, "Failed to allocate memory");
    exit(1);
  }
  memset(arr->data + arr->max_size, 0, arr->max_size);

  arr->max_size *= 2;
  printf("Resized to %i\n", arr->max_size);
}

void addChar(dynamicArray arr, char element) {
  if (arr->current_size + 1 >= arr->max_size) {
    //reallocate more memory
    resizeArray(arr);
  }
  arr->data[arr->current_size++] = element;
}

void addString(dynamicArray arr, char *str) {
  while (*str != '\0') {
    addChar(arr, *str);
    str++;
  }
}

char getChar(dynamicArray arr, int index) {
  return arr->data[index];
}

char *getString(dynamicArray arr) {
  return arr->data;
}

void freeDynamicArray(dynamicArray arr) {
  free(arr->data);
  free(arr);
}

int main(void) {
  return 0;
}
