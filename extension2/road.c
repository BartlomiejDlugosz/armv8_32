#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>
#include "road.h"
// check lights (just once)
// update distance of all cars (starting with first)
// if distance stays the same, add time to stationary time
 // update_distances(road, delta_t); // let cars roll forward if possible (note special case for first car)
void *update_distances(road* update_road, time_t dt)
{   
    int speed_limit = update_road->speed_limit;
    // the lights can be checked using the road (it has a light attribute)
    int distance_covered = speed_limit * dt;
    if (update_road->light.clr == RED) {
        // dealing with HEAD
        int cur_distance = update_road->head_car->distance_to_car_in_front; // distance to light
        // distance = speed * time
        int new_distance = cur_distance - distance_covered;
        update_road->head_car->distance_to_car_in_front = MAX(new_distance, 0);
        bool all_cars_in_front_are_stationary = false;
        if (cur_distance == new_distance) {
            all_cars_in_front_are_stationary = true;
        }
        distance_covered -= cur_distance - new_distance;
        // Dealing with rest of the cars
        car* prev_car = update_road->head_car;
        car* next_car = update_road->head_car->next;
        int follow_distance = update_road->follow_distance; // minimum separation needed between cars
        while (next_car != NULL && distance_covered != 0) {

            int distance_to_car_in_front_prev = next_car->distance_to_car_in_front; // previous 
            next_car->distance_to_car_in_front = MAX(follow_distance, next_car->distance_to_car_in_front - distance_covered);
            distance_covered -= distance_to_car_in_front_prev - next_car->distance_to_car_in_front;
            if (next_car->distance_to_car_in_front == distance_to_car_in_front_prev && all_cars_in_front_are_stationary) {
                next_car->time_stationary += dt;
            } else {
                all_cars_in_front_are_stationary = false;
            }

            next_car = next_car->next;

        }

    } else if (update_road->light.clr == GREEN) {
        update_road->head_car->distance_to_car_in_front -= distance_covered; //distance to light can now become negative
    }
}

// pop off ANY cars which have passed stop line (checking for negative distances), that involves
// potentially popping many cars, updating the running head and its distance to the light
// return the head of number of cars crossed
// head_of_crossed = remove_crossed(road); // pop off ANY cars which have passed stop line. return the number of cars that crossed
car *remove_crossed(road cur_road)
{
    return NULL;
}

bool maybe_add_cars(road *update_road)
{
    // true iff a car was added
    
    // called with a certain probability in simulate_traffic.
    // check if there is space on the road for a new car, 
    // in which case add it

    car *current_car = update_road->head_car;

    int total_distance = 0;
    // special case where no cars on road:
    if (current_car == NULL) {
        // skip while loop, no cars on road
    } else {
        while (current_car->next != NULL) {
            total_distance += current_car->distance_to_car_in_front;
            current_car = current_car->next;
        }
        if (total_distance >= update_road->length) {
            return false; // no space for car
        }
    }
    
    car *new_car = malloc(sizeof(struct car));
    if (new_car == NULL) {
        printf("road: failed to initialise car!");
        return false;
    }

    new_car->distance_to_car_in_front = update_road->length - total_distance;
    new_car->next = NULL;
    new_car->time_stationary = 0;

    if (update_road->head_car == NULL) {
        update_road->head_car = new_car;
    } else {
        current_car->next = new_car;
    }
    return true;
}


// make sure you keep track of the next car in line, 
// since if you don't you'll lose access to it after
// freeing the current_car
void free_single_car(car *current_car) {
    free(current_car);
}


void free_all_cars(car *current_car) {
    if (current_car->next != NULL) {
        free_all_cars(current_car->next);
    }
    free(current_car);
}
