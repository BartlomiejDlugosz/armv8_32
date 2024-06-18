#ifndef CAR_H
#define CAR_H

typedef struct car {
    int distance_to_car_in_front; // distance_to_light if HEAD
    struct car *next; // possibly null
} car;

#endif // CAR_H
