#include "register_to_number.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Converts a register of the form "Rx" where x is an int to simply x as an int,
// or -1 if z register, or -2 if stack pointer
int register_to_number(char *reg) {
    int result;
    if (strncmp(reg + 1, "zr", 2) == 0)
        result = 31;
    else if (strcmp(reg, "SP") == 0)
        result = -1;
    else {
        int j = 0;  // index for output array
        char output[256];
        for (int i = 0; reg[i] != '\0'; i++) {
            if (isdigit((unsigned char)reg[i])) {
                output[j++] = reg[i];
            }
        }
        output[j] = '\0';  // null-terminate the output string
        result = atoi(output);
    }
    return result;
}