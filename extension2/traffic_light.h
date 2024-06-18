#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <stdbool.h>
#include "traffic_light.h"


typedef enum { RED, RED_AMBER, GREEN, AMBER} colour;
typedef enum { LEFT, RIGHT } direction ;

typedef struct {
    colour clr;
    bool has_arrow;
    bool arrow_lit;
    direction arrow;
    bool sensor; // tells us if cars are waiting
} light;

void update_lights_to_next_state();

#endif // TRAFFIC_LIGHT_H
