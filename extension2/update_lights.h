#ifndef UPDATE_LIGHTS_H
#define UPDATE_LIGHTS_H
#include <stdbool.h>
#include <time.h>

#include "intersection.h"
#include "strategies.h"

bool update_lights_to_next_state(intersection *isec, time_t dt,
                                 time_t *time_since_change, strategy s,
                                 Chromosome *optimal_data);

#endif  // UPDATE_LIGHTS_H
