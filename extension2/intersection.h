#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "road.h"

typedef struct {
    road roads[4];

    // make sure these stay in sync
    light lights[4];
    int state_index;
} intersection;

#endif // INTERSECTION_H
