#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "road.h"

typedef struct {
    road *roads[4];

    // make sure these stay in sync
    // traffic_light lights[4];
    int state_index;
} intersection;

void print_intersection(intersection *isec);

#endif  // INTERSECTION_H
