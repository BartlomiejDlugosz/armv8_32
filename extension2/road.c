#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>
#include "road.h"

void update_distances(road* update_road, time_t dt) {   
    if (update_road->head_car == NULL) return;
    int speed_limit = update_road->speed_limit;
    // Maximum distance a car is able to cover in the time and speed limit given
    int distance_covered = speed_limit * dt;
    // Dealing with case light is red
    if (update_road->light->clr == RED) {
        // Dealing with the HEAD
        int cur_distance = update_road->head_car->distance_to_car_in_front;
        int new_distance = cur_distance - distance_covered;
        // Either the HEAD reaches the light or covers some distance
        update_road->head_car->distance_to_car_in_front = MAX(new_distance, 0);
        // If the cars in front are stationary, the cars behind can still move
        // If the cars in front are not stationary, the cars behind still move regardless
        // So don't add time stationary for any cars behind
        bool all_cars_in_front_are_stationary = false;
        // If the car hasn't moved, add to time stationary (same logic is used in while loop)
        if (cur_distance == update_road->head_car->distance_to_car_in_front) {
            all_cars_in_front_are_stationary = true;
        }
        // The distance covered is now reduced by the distance covered by the HEAD car
        distance_covered -= cur_distance - update_road->head_car->distance_to_car_in_front;
        // Dealing with the rest of the cars
        car* next_car = update_road->head_car->next;
        int follow_distance = update_road->follow_distance;
        //Distances are relative so only cover the distance available
        while (next_car != NULL && distance_covered > 0) {
            // Previous Distance
            int distance_to_car_in_front_prev = next_car->distance_to_car_in_front;
            // Either the reaches the maximum follow distance allowed or covers some distance
            next_car->distance_to_car_in_front = MAX(follow_distance, next_car->distance_to_car_in_front - distance_covered);
            // Distance covered is decreased by the distance covered by the car
            distance_covered -= distance_to_car_in_front_prev - next_car->distance_to_car_in_front;
            // If the cars in front have not moved and the distance covered by the car is 0, it has remained stationary
            if (next_car->distance_to_car_in_front - distance_to_car_in_front_prev == 0 && all_cars_in_front_are_stationary) {
                // So add to time stationary
                next_car->time_stationary += dt;
            } 
            // Otherwise, the car has moved so update the boolean
            else {
                all_cars_in_front_are_stationary = false;
            }
            // Move onto the next car
            next_car = next_car->next;
        }
    } 
    // Other light cases mean the cars will go (RED_AMBER, AMBER, GREEN)
    else {
        // Only need to update HEAD car as distances are relative
        // They all move the saem distance
        update_road->head_car->distance_to_car_in_front -= distance_covered;
    }
}

// pop off ANY cars which have passed stop line (checking for negative distances), that involves
// potentially popping many cars, updating the running head and its distance to the light
car* remove_crossed(road* cur_road) {
    // If the road is empty, return NULL
    if (cur_road->head_car == NULL) {
        return NULL;
    }
    // Track the old head of the car
    // This will be useful to return the linked list of cars that have crossed
    car* old_head = cur_road->head_car;
    // Keep track of last popped inorder to complete the linked list of cars that have crossed
    car* last_popped = NULL;
    int distance_travelled = cur_road->head_car->distance_to_car_in_front;
    // If the distance travelled is greater than zero, this means that the car has to still reach the light
    // Remember for the HEAD car, the distance to car in front represents the distance to the light
    // If distance travelled is zero, the car is at the traffic light and has not yet crossed
    if (distance_travelled >= 0) {
        return NULL;
    }
    // Dealing with the rest of the cars
    while (distance_travelled < 0 && cur_road->head_car != NULL) {
        // Check if there is a next car, in which case we need to update distance travelled
        if (cur_road->head_car->next != NULL) {
            distance_travelled += cur_road->head_car->next->distance_to_car_in_front;
        }
        // Update last popped
        last_popped = cur_road->head_car;
        cur_road->num_cars--;
        // Move onto the next car
        cur_road->head_car = cur_road->head_car->next;
    }

    // Ensure the linked list is completed by making last_popped->next NULL
    if (last_popped != NULL) {
        last_popped->next = NULL;
    }

    // If there is a head, then update the distance
    if (cur_road->head_car != NULL)  {
        cur_road->head_car->distance_to_car_in_front -= distance_travelled;
    }
    // Otherwise there is no need to update distance

    return old_head;    

}

bool maybe_add_car(road *update_road)
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

    update_road->num_cars++;

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
    printf("road len: %i, speed: %d, num_cars: %d\n", rd->length, rd->speed_limit, rd->num_cars);
    printf("NULL");
    print_car(rd->head_car);
    printf(" | ");
    print_traffic_light(rd->light);
    printf("\n\n");
}
