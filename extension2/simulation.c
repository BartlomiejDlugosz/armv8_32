#include <stdlib.h>
#include <string.h>
#include "simulate_traffic.h"
#include "strategies.h"
#define NUM_STRATEGY_CALLS 100

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
    int basic_durations[NUM_STATES] = {TIME_TO_CHANGE_BASIC, TIME_TO_CHANGE_AMBER, TIME_TO_CHANGE_AMBER, TIME_TO_CHANGE_BASIC, TIME_TO_CHANGE_AMBER, TIME_TO_CHANGE_AMBER};
    Chromosome optimal_data_struct;
    Chromosome *optimal_data;
    optimal_data = &optimal_data_struct;
    optimal_data->durations[0] = basic_durations[0];
    optimal_data->durations[1] = basic_durations[1];
    optimal_data->durations[2] = basic_durations[2];
    optimal_data->durations[3] = basic_durations[3];
    optimal_data->durations[4] = basic_durations[4];
    optimal_data->durations[5] = basic_durations[5];

    FILE* output_file = fopen(argv[1], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return EXIT_FAILURE;
    }

    // performance_evaluation will return a pointer to intersection evaluation withe parameters mentioned above
    if (argc == 2) {
        for (int i = 0; i < NUM_STRATEGIES; i++) {
            const strategy s = strategies[i];
            const char* strategy_name = strategy_names[i];
            double total_average_time_stationary = 0;
            double total_maximum_time_stationary = 0;
            for (int i = 0; i < NUM_STRATEGY_CALLS; i++) {
                intersection_evaluation* returned_evaluation = simulate_traffic(s, optimal_data);
                total_average_time_stationary += (double)(returned_evaluation->total_average_time_stationary);
                total_maximum_time_stationary += (double)(returned_evaluation->total_maximum_time_stationary);
            }
            total_average_time_stationary /= NUM_STRATEGY_CALLS;
            total_maximum_time_stationary /= NUM_STRATEGY_CALLS;
            fprintf(output_file, "Strategy: %s Total Average Time: %lf, Total Maximum Time: %lf\n", strategy_name, total_average_time_stationary, total_maximum_time_stationary);
        }

        fclose(output_file);
        return EXIT_SUCCESS;
    } else if (argc == 3) {
        const char* command = argv[2];
        if (strcmp(command, "train_model_on_avg") == 0) {
            // Call function to train model on average
        }
        else if (strcmp(command, "train_model_on_max") == 0) {
            // Call function on train model on max
        }
        else {
            // Assume strategy
            const char* strategy_name = command;
            bool strategy_found = false;
            for (int i = 0; i < NUM_STRATEGIES; i++) {
                if (strcmp(strategy_name, strategy_names[i]) == 0) {
                    const strategy s = strategies[i];
                    intersection_evaluation* returned_evaluation = simulate_traffic(s, optimal_data);
                    double total_average_time_stationary = (double)(returned_evaluation->total_average_time_stationary);
                    double total_maximum_time_stationary = (double)(returned_evaluation->total_maximum_time_stationary);
                    fprintf(output_file, "Strategy: %s Total Average Time: %lf, Total Maximum Time: %lf\n", strategy_name, total_average_time_stationary, total_maximum_time_stationary);
                    strategy_found = true;
                    break;
                }
            }
            if (!strategy_found) {
                fprintf(stderr, "Unknown strategy: %s\n", strategy_name);
                fclose(output_file);
                return EXIT_FAILURE;
            }
        }
    } else {
        fprintf(stderr, "Incorrect command line input. See README for details on inputs");
        return EXIT_FAILURE;
    }

    return 0;
}