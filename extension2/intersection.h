#ifndef INTERSECTION_H
#define INTERSECTION_H


#include "road.h"

typedef struct {
    road roads[4];
    traffic_light lights[4];
} intersection;

#endif // INTERSECTION_H
