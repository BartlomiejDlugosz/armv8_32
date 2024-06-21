#include <stdbool.h>

#include "update_lights.h"
#include "traffic_light.h"

bool update_lights_to_next_state(intersection *isec, time_t dt,
                                 time_t *time_since_change, strategy target_strategy,
                                 Chromosome *optimal_data) {
    if ((target_strategy)(isec, *time_since_change, optimal_data)) {
        isec->state_index = (isec->state_index + 1) % NUM_STATES;
        for (int i = 0; i < NUM_ROADS; i++) {
            isec->roads[i]->light->clr = states[isec->state_index][i];
        }
        *time_since_change = 0;
        return true;
    }

    *time_since_change += dt;
    return false;

}
