#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#include "update_lights.h"

bool update_lights_to_next_state(intersection *isec, time_t dt, time_t *time_since_change, strategy s) {
    if ((s) (isec, time_since_change)) {
        isec->state_index = (isec->state_index + 1) % (NUM_STATES + 1);
        for(int i = 0; i < NUM_ROADS; i++) {
            printf("Road: %i, %d -> ", i, isec->roads[i].light.clr);
            isec->roads[i].light.clr = states[isec->state_index][i];
            printf("%d\n", isec->roads[i].light.clr);
        }
        printf("\n\n");
        *time_since_change = 0;
        return true;
    }
    else {
        printf("Time since last change: %li\n", *time_since_change);
        *time_since_change += dt;
        return false;
    }
}