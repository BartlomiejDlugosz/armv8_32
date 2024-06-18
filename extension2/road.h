#ifndef ROAD_H
#define ROAD_H

#include "traffic_light.h"
#include "car.h"

typedef struct {
    int length; // traverse the ll, only adding if sum < length
    int speed_limit;
    car *head_car;
    traffic_light light;
} road;

// think about case where road->first_car == NULL
// in the case where nothing is updated, the previous
// head is returned so use as
// road->first_car = maybe_add_cars(road, f(x));
car *maybe_add_cars();

#endif // ROAD_H
