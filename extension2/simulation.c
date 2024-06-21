#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "simulate_traffic.h"
#include "strategies.h"

#define NUM_STRATEGY_CALLS 100

// One strategy
// ./simulation (FILE) strategy

//  ./simulation (FILE)

// Call simulate traffic multiple times eg 10 times with a certain max iteration
// in simulate_traffic.c Know the avg stationary time for each road and the max

// ./simulation (FILE) train_model_on_avg
// ./simulation (FILE) train_model_on_max
// call the function with the parameters (avg stationary time for each road and
// the max) do not call it is trained
int main(int argc, char** argv) {
    srand(time(NULL));  // Initialization, should only be called once.
    // Calling all strategies and writing to a file
    // ./simulation (FILE)
    int basic_durations[NUM_STATES] = {
        TIME_TO_CHANGE_BASIC, TIME_TO_CHANGE_AMBER, TIME_TO_CHANGE_AMBER,
        TIME_TO_CHANGE_BASIC, TIME_TO_CHANGE_AMBER, TIME_TO_CHANGE_AMBER};

    // Defines a chromosone which will hold the optimal durations
    // Initialise it with default durations
    Chromosome optimal_data = {0};
    for (int i = 0; i < NUM_STATES; i++) {
        optimal_data.durations[i] = basic_durations[i];
    }

    // Open file which will be used to output the results
    FILE* output_file = fopen(argv[1], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return EXIT_FAILURE;
    }

    // performance_evaluation will return a pointer to intersection evaluation
    // with the parameters mentioned above
    if (argc == 2) {
        for (int i = 0; i < NUM_STRATEGIES; i++) {
            // Gets the current strategy and name
            strategy target_strategy = strategies[i];
            char* strategy_name = strategy_names[i];

            // If one of genetic algorithms then it trains them
            if (strcmp(strategy_name, "Genetic Algorithm Avg") == 0) {
                optimal_data = train_genetic_algorithm(true);
            } else if (strcmp(strategy_name, "Genetic Algorithm Max") == 0) {
                optimal_data = train_genetic_algorithm(false);
            }

            // Evaluates the performance of the strategy
            double total_average_time_stationary = 0;
            double total_maximum_time_stationary = 0;

            // Simulates the given strategy a number of times and calculates an average
            for (int j = 0; j < NUM_STRATEGY_CALLS; j++) {
                intersection_evaluation* returned_evaluation =
                    simulate_traffic(target_strategy, &optimal_data, strategy_name);
                if (returned_evaluation == NULL) {
                    fclose(output_file);
                    return EXIT_FAILURE;
                }

                total_average_time_stationary +=
                    returned_evaluation
                                 ->total_average_time_stationary;
                total_maximum_time_stationary +=
                    (double)returned_evaluation
                                 ->total_maximum_time_stationary;
                // Free the malloced intersection
                free_isec_eval(returned_evaluation);
            }
            total_average_time_stationary /= NUM_STRATEGY_CALLS;
            total_maximum_time_stationary /= NUM_STRATEGY_CALLS;

            // Outputs the results to the file
            fprintf(output_file,
                    "Strategy: %s Total Average Time: %lf, Total Maximum Time: "
                    "%lf\n",
                    strategy_name, total_average_time_stationary,
                    total_maximum_time_stationary);
        }
        fclose(output_file);
        return EXIT_SUCCESS;
    }
    if (argc == 3) {
        // Get the strategy name from the command line argumantes
        char* strategy_name = argv[2];
        // Check if related to genetic algorithm
        if (strcmp(strategy_name, "train_model_on_avg") == 0) {
            optimal_data = train_genetic_algorithm(true);
            strategy_name = "Genetic Algorithm with avg";
        }
        if (strcmp(strategy_name, "train_model_on_max") == 0) {
            optimal_data = train_genetic_algorithm(false);
            strategy_name = "Genetic Algorithm with max";
        }

        // Finds the strategy passed in through the command line
        bool strategy_found = false;
        for (int i = 0; i < NUM_STRATEGIES; i++) {
            if (strcmp(strategy_name, strategy_names[i]) == 0) {
                // Simulates the target strategy
                strategy target_strategy = strategies[i];

                intersection_evaluation* returned_evaluation =
                    simulate_traffic(target_strategy, &optimal_data, strategy_name);
                if (returned_evaluation == NULL) {
                    fclose(output_file);
                    return EXIT_FAILURE;
                }

                double total_average_time_stationary =
                    returned_evaluation
                                 ->total_average_time_stationary;
                double total_maximum_time_stationary =
                    (double)returned_evaluation
                                 ->total_maximum_time_stationary;

                free_isec_eval(returned_evaluation);

                fprintf(output_file,
                        "Strategy: %s Total Average Time: %lf, Total Maximum "
                        "Time: %lf\n",
                        strategy_name, total_average_time_stationary,
                        total_maximum_time_stationary);
                strategy_found = true;
                break;
            }
        }
        // Throw error if strategy not foudn
        if (!strategy_found) {
            fprintf(stderr, "Unknown strategy: %s\n", strategy_name);
            fclose(output_file);
            return EXIT_FAILURE;
        }

        fclose(output_file);
        return EXIT_SUCCESS;
    }

    // Unknown command line arguments
    fprintf(stderr,"Incorrect command line input. See README for details on inputs");
    fclose(output_file);
    return EXIT_FAILURE;
}
