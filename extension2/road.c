#include "road.h"
#include <sys/param.h>

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

car *maybe_add_cars()
{
    return NULL;
}