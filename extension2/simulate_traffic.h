#ifndef SIMULATE_TRAFFIC_H
#define SIMULATE_TRAFFIC_H

#include <stdio.h>
#include "strategies.h"
#include "performance_evaluation.h"

intersection_evaluation* simulate_traffic(strategy s, Chromosome *optimal_data, char * strategy_name);

#endif
