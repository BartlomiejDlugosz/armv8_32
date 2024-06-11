#include "register_to_number.h"
#include <string.h>
#include <stdlib.h>
// Function to remove a specified prefix from a string
char* remove_non_integer_characters(char *input) {
    int j = 0; // index for output array
    char *output;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isdigit((unsigned char)input[i])) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0'; // null-terminate the output string
}

uint8_t register_to_number(char *reg) {
    uint8_t result = atoi(remove_non_integer_characters(reg));
    return result;
}