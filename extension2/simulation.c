#include <stdlib.h>
#include "simulate_traffic.h"
#include "strategies.h"

#define NUM_STRATEGY_CALLS 10
#define NUM_STRATEGIES 4

// One strategy
// ./simulation (FILE) strategy

//  ./simulation (FILE)

// Call simulate traffic multiple times eg 10 times with a certain max iteration in simulate_traffic.c
// Know the avg stationary time for each road and the max

// ./simulation (FILE) train_model_on_avg
// ./simulation (FILE) train_model_on_max
// call the function with the parameters (avg stationary time for each road and the max)
// do not call it is trained
int main(int argc, char **argv) {

    // Calling all strategies and writing to a file
    // ./simulation (FILE)

    FILE* output_file = fopen(argv[1], "a");
    if (output_file == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return EXIT_FAILURE;
    }

    // performance_evaluation will return a pointer to intersection evaluation withe parameters mentioned above
    if (argc == 2) {
        for (int i = 0; i < NUM_STRATEGIES; i++) {
            for (int i = 0; i < NUM_STRATEGY_CALLS; i++) {
                break;
            }
        }
    }

    // Calling one strategy only and that to only once
    // ./simulation (FILE) strategy

    return 0;
}