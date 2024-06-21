#include <stdio.h>
#include "traffic_light.h"

// Source for terminal colours:
// https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_ORANGE "\x1b[38;5;214m"
#define ANSI_COLOR_RESET "\x1b[0m"

void print_traffic_light(traffic_light *light) {
    switch (light->clr) {
        case 0:
            printf(ANSI_COLOR_RED "RED " ANSI_COLOR_RESET "\n");
            break;
        case 1:
            printf(ANSI_COLOR_RED "RED" ANSI_COLOR_RESET "");
            printf(ANSI_COLOR_ORANGE "_AMBER" ANSI_COLOR_RESET "\n");
            break;
        case 2:
            printf(ANSI_COLOR_GREEN "GREEN " ANSI_COLOR_RESET "\n");
            break;
        case 3:
            printf(ANSI_COLOR_ORANGE "AMBER" ANSI_COLOR_RESET "\n");
            break;
    }
}
