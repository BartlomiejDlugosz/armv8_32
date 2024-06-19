#ifndef ROAD_H
#define ROAD_H

#include "car.h"
#include "traffic_light.h"

typedef struct {
    int length; // traverse the ll, only adding if sum < length
    int speed_limit;
    int follow_distance; //minimum distance a car be from another car
    car *head_car;
    traffic_light light;
} road;



// check lights (just once)
// update distance of all cars (starting with first)
// if distance stays the same, add time to stationary time
void update_distances(road *update_road, time_t dt);

// pop off ANY cars which have passed stop line (checking for negative distances), that involves
// potentially popping many cars, updating the running head and its distance to the light
// return the head of number of cars crossed
car* remove_crossed(road* cur_road);

bool maybe_add_cars();

void free_single_car(car *current_car);

void free_all_cars(car *current_car);

void print_road(road *rd);

#endif // ROAD_H
