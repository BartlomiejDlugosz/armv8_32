#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
#include "states.h"


typedef struct {
    int durations[NUM_STATES];
    double fitness;
} Chromosome;

Chromosome *train_genetic_algorithm(bool is_avg);

#endif // GENETIC_ALGORITHM_H