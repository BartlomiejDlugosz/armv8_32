#ifndef CAR_H
#define CAR_H

#include <time.h>
#include "road.h"

typedef struct car {
    int distance_to_car_in_front; // distance_to_light if HEAD
    struct car *next; // possibly null
    time_t time_stationary; // time stationary initially 0
} car;

// need to keep track how long they are waiting for


// check lights (just once)
// update distance of all cars (starting with first)
// if distance stays the same, add time to stationary time
void *update_distances();

// pop off ANY cars which have passed stop line (checking for negative distances), that involves
// potentially popping many cars, updating the running head and its distance to the light
// return the head of number of cars crossed
car* remove_crossed();

#endif // CAR_H
