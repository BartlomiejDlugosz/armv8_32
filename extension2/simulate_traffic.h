#ifndef SIMULATE_TRAFFIC_H
#define SIMULATE_TRAFFIC_H

#include <stdio.h>
#include "strategies.h"
#include "performance_evaluation.h"
void free_isec_eval(intersection_evaluation *isec_eval);

void initialise_isec_eval(intersection_evaluation *isec_eval);

intersection_evaluation *simulate_traffic(strategy target_strategy, Chromosome* optimal_data,
                                          char* strategy_name);

#endif
