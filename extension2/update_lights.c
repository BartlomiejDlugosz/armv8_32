#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#include "update_lights.h"
#include "traffic_light.h"

bool update_lights_to_next_state(intersection *isec, time_t dt,
                                 time_t *time_since_change, strategy s,
                                 Chromosome *optimal_data) {
    if ((s)(isec, *time_since_change, optimal_data)) {
        isec->state_index = (isec->state_index + 1) % (NUM_STATES);
        for (int i = 0; i < NUM_ROADS; i++) {
            // printf("Road: %i ", i);
            // print_traffic_light(isec->roads[i]->light);
            isec->roads[i]->light->clr = states[isec->state_index][i];
            // printf(" -> ");
            // print_traffic_light(isec->roads[i]->light);
            // printf("\n");
        }
        // printf("\n\n");
        *time_since_change = 0;
        // also update physical LEDs
        // update_leds(isec->state_index); UNCOMMENT
        return true;
    } else {
        // printf("Time since last change: %li\n", *time_since_change);
        *time_since_change += dt;
        return false;
    }
}
