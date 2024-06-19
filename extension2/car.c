#include <stdio.h>
#include "car.h"

void print_car(car *head_car) {
    if(head_car == NULL) {
        return;
    }
    printf("Distance to next: %i. Time stationary: %li\n", head_car->distance_to_car_in_front, head_car->time_stationary);
    print_car(head_car->next);
}