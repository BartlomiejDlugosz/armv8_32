#include <stdio.h>
#include "car.h"

void print_car(car *head_car) {
    if (head_car == NULL) {
        return;
    }
    print_car(head_car->next);
    printf(" <- CAR[%im, %lis]", head_car->distance_to_car_in_front,
           head_car->time_stationary);
}
