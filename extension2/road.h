#ifndef ROAD_H
#define ROAD_H

#include "car.h"
#include "traffic_light.h"

typedef struct {
    int length;
    int speed_limit;
    int follow_distance;
    car *head_car;
    traffic_light *light;
    int num_cars;
} road;

void update_distances(road *road_to_update, time_t dt);

car *remove_crossed(road *cur_road);

bool maybe_add_car(road *update_road);

void free_single_car(car *current_car);

void free_all_cars(car *current_car);

void print_road(road *rd);

#endif  // ROAD_H
