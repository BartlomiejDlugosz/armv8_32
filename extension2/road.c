#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>
#include "road.h"
// check lights (just once)
// update distance of all cars (starting with first)
// if distance stays the same, add time to stationary time
 // update_distances(road, delta_t); // let cars roll forward if possible (note special case for first car)
void update_distances(road* update_road, time_t dt) {   
    if (update_road->head_car == NULL) return; //early return
    int speed_limit = update_road->speed_limit;
    // the lights can be checked using the road (it has a light attribute)
    int distance_covered = speed_limit * dt;
    if (update_road->light->clr == RED) {
        // dealing with HEAD
        int cur_distance = update_road->head_car->distance_to_car_in_front; // distance to light
        int new_distance = cur_distance - distance_covered;
        update_road->head_car->distance_to_car_in_front = MAX(new_distance, 0);
        bool all_cars_in_front_are_stationary = false;
        if (cur_distance == new_distance) {
            all_cars_in_front_are_stationary = true;
        }
        distance_covered -= cur_distance - update_road->head_car->distance_to_car_in_front;
        // Dealing with rest of the cars
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

    } else { // if it's anything else we can just go ie red_amber or amber or green
    // might need to change if acceleration is taken into account
        update_road->head_car->distance_to_car_in_front -= distance_covered; //distance to light can now become negative
    }
}

// pop off ANY cars which have passed stop line (checking for negative distances), that involves
// potentially popping many cars, updating the running head and its distance to the light
car* remove_crossed(road* cur_road) {
    if (cur_road->head_car == NULL) {
        return NULL;
    }

    car* old_head = cur_road->head_car;
    car* last_popped = NULL;
    int distance_travelled = cur_road->head_car->distance_to_car_in_front;

    if (distance_travelled >= 0) {
        return NULL;
    }

    while (distance_travelled < 0 && cur_road->head_car != NULL) {
        // check if there is a next car, in which case we need to update distance travelled
        if (cur_road->head_car->next != NULL) {
            distance_travelled += cur_road->head_car->next->distance_to_car_in_front;
        }

        last_popped = cur_road->head_car;
        cur_road->head_car = cur_road->head_car->next;
    }

    // if only one car crossed
    if (last_popped != NULL) {
        last_popped->next = NULL;
    }

    // if there is a head, then update dist
    if (cur_road->head_car != NULL)  {
        cur_road->head_car->distance_to_car_in_front -= distance_travelled;
    }
    // otherwise no head, so no need to update dist

    return old_head;    

}

bool maybe_add_cars(road *update_road)
{
    // true iff a car was added
    
    // called with a certain probability in simulate_traffic.
    // check if there is space on the road for a new car, 
    // in which case add it

    car *current_car = update_road->head_car;
    car *prev_car = NULL;

    int total_distance = 0;
    while (current_car != NULL) {
        total_distance += current_car->distance_to_car_in_front;
        prev_car = current_car;
        current_car = current_car->next;
    }
    // current_car = NULL
    // prev_car is last car in ll

    if (total_distance >= update_road->length) {
        return false; // no space for car
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
        prev_car->next = new_car;
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
    if (current_car == NULL) {
        return;
    }
    if (current_car->next != NULL) {
        free_all_cars(current_car->next);
    }
    free(current_car);
}

void print_road(road *rd) {
    printf("road len: %i speed: %d\n", rd->length, rd->speed_limit);
    printf("NULL");
    print_car(rd->head_car);
    printf(" | ");
    print_traffic_light(rd->light);
    printf("\n\n");
}
