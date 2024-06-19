#include <time.h>
#include <stdbool.h>

#include "strategies.h"

static bool cars_waiting(intersection *isec) {
    bool cars_waiting_bool = false;
    for(int i = 0;  i < NUM_ROADS; i++) {
        cars_waiting_bool |= (isec->roads[i]->light->clr == RED && isec->roads[i]->head_car != NULL);
    }
    return cars_waiting_bool;
}

static bool amber_lights(intersection *isec) {
    bool amber_light = false;
    for(int i = 0;  i < NUM_ROADS; i++) {
        amber_light |= (isec->roads[i]->light->clr == RED_AMBER || isec->roads[i]->light->clr == AMBER);
    }
    return amber_light;
}

bool basic (intersection *isec, time_t time_since_change) {
    if (amber_lights(isec) && time_since_change > 2) {
        return true;
    } else if (time_since_change > 15) {
        return true;
    }
    return false;
}

bool basic_plus (intersection *isec, time_t time_since_change) {
    if (amber_lights(isec) && time_since_change > 2) {
        return true;
    } else if (time_since_change > 15) {
        if(cars_waiting(isec)) {
            return true;
        }
        return true;
    }
    return false;
}
