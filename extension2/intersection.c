#include <stdbool.h>
#include "intersection.h"
#include <time.h>
#include "states.h"
#include "strategies.h"
/*
bool update_lights_to_next_state(strategy *s) {
    return true;
}
*/

bool update_lights_to_next_state(intersection *isec, time_t dt, time_t *time_since_change, strategy *s) {
    if ((*s) (isec, time_since_change)) {

    }
}