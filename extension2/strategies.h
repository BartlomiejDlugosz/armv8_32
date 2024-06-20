#ifndef STRATEGIES_H
#define STRATEGIES_H
#include <time.h>
#include <stdbool.h>
#include "states.h"
#include "intersection.h"
#include "genetic_algorithm.h"
#define NUM_STRATEGIES 4
#define TIME_TO_CHANGE_BASIC 30
#define TIME_TO_CHANGE_AMBER 2
// every strategy:
// 1. intersection (to access lights and state)
// 2. takes in the time since change

typedef bool (*strategy)(intersection *isec, time_t time_since_change, Chromosome *optimal_data);

bool basic (intersection *isec, time_t time_since_change, Chromosome *optimal_data);

bool basic_plus (intersection *isec, time_t time_since_change, Chromosome *optimal_data);

bool genetic_algorithm (intersection *isec, time_t time_since_change, Chromosome *optimal_data);

extern strategy strategies[NUM_STRATEGIES];
extern char *strategy_names[NUM_STRATEGIES];
// takes in a number of parameters which are to be decided
// note that the parameters need to be the same for all strategies
// so update the above typedef to accomodate this!
//
// simulate_traffic then takes in a strategy to decide
// when to change the state of the lights
// returns a bool where true means cycle to next stage in light
//
// We can check this with a helper function, which is 
// defined inside of strategies.c:

#endif // STRATEGIES_H
