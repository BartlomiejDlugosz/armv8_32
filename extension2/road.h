#ifndef ROAD_H
#define ROAD_H
#include <stdint.h>
#include "car.h"
#include "traffic_light.h"

typedef struct {
    uint64_t length;
    int speed_limit;
    int follow_distance;
    car *head_car;
    traffic_light *light;
} road;

void update_distances(road *update_road, time_t dt);

car* remove_crossed(road* cur_road);

bool maybe_add_cars();

void free_single_car(car *current_car);

void free_all_cars(car *current_car);

void print_road(road *rd);

#endif // ROAD_H
