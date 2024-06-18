#ifndef CAR_H
#define CAR_H

#include "road.h"

typedef struct car {
    int distance_to_car_in_front; // distance_to_light if HEAD
    struct car *next; // possibly null
} car;


// check lights (just once)
// update distance of all cars (starting with first)
void *update_distances();

// pop off ANY cars which have passed stop line, that involves
// potentially popping many cars, updating the running head
// return head
car *remove_crossed();

#endif // CAR_H
