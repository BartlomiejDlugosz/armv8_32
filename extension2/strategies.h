#ifndef STRATEGIES_H
#define STRATEGIES_H
#include <time.h>
#include <stdbool.h>
#include "states.h"
#include "intersection.h"
// every strategy:
// 1. intersection (to access lights and state)
// 2. takes in the time since change

typedef bool (*strategy)(intersection *isec, time_t time_since_change);

bool basic (intersection *isec, time_t time_since_change);

bool basic_plus (intersection *isec, time_t time_since_change);

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
