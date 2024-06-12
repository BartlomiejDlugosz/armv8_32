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

// Converts a register of the form "Rx" where x is an int to simply x as an int, or -1 if z register, or -2 if stack pointer
int register_to_number(char *reg) {
    if(strncmp(reg+1, "zr", 2) == 0)
        return 31;
    else if (strcmp(reg, "SP") == 0)
        return -2;
    int result = atoi(remove_non_integer_characters(reg));
    return result;
}