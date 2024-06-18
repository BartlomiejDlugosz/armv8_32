#ifndef CAR_H
#define CAR_H

#include <time.h>

typedef struct car {
    int distance_to_car_in_front; // distance_to_light if HEAD
    struct car *next; // possibly null
    time_t time_stationary; // time stationary initially 0
} car;

#endif // CAR_H
