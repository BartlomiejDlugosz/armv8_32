#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h> // debugging

#include "traffic_light.h"
#include "states.h"
#include "road.h"
#include "intersection.h"
#include "strategies.h"
#include "update_lights.h"

#define N 5

int main(int argc, char **argv) {

    srand(time(NULL));   // Initialization, should only be called once.

    // initialise all the structures
    traffic_light light0 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    traffic_light light1 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};
    traffic_light light2 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    traffic_light light3 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};

    road road0 = {.length = 10, .speed_limit = 60, .follow_distance = 2 , .head_car = NULL, .light = &light0 };
    road road1 = {.length = 100, .speed_limit = 50, .follow_distance = 2 , .head_car = NULL, .light = &light1 };
    road road2 = {.length = 20, .speed_limit = 10, .follow_distance = 2 , .head_car = NULL, .light = &light2 };
    road road3 = {.length = 100, .speed_limit = 50, .follow_distance = 2 , .head_car = NULL, .light = &light3 };
    
    intersection isec_struct;
    intersection *isec;

    isec = &isec_struct;
    isec->roads[0] = &road0;
    isec->roads[1] = &road1;
    isec->roads[2] = &road2;
    isec->roads[3] = &road3;
    isec->state_index = 0;

    strategy s = basic;
    
    road *current_road;
    car *head_of_crossed;
    time_t initial_time_since_change = 0;
    time_t *time_since_change;
    time_since_change = &initial_time_since_change;

    time_t dt = 1; // seconds
    uint64_t max_iterations = 100;

    for (uint64_t iter = 0; iter < max_iterations; iter++) { // timestep
        printf("\n\n\n\nSTART OF ITERATION\n");
        print_intersection(isec);

        update_lights_to_next_state(isec, dt, time_since_change, s); // takes a strategy
        
        for (int i = 0; i < NUM_ROADS; i++) {
            current_road = isec->roads[i];

            update_distances(current_road, dt); // let cars roll forward if possible (note special case for first car)
            head_of_crossed = remove_crossed(current_road); // pop off ANY cars which have passed stop line. return the number of cars that crossed
            free_all_cars(head_of_crossed); // because we don't calculate best algo yet

            if (rand() < (RAND_MAX+1u) / N) {// perform with probability 1/N
                maybe_add_cars(current_road); // also checks sum < length of road
            }
        }
    }

    for (int i = 0; i < NUM_ROADS; i++) {
        free_all_cars(isec->roads[i]->head_car);
    }
    return 0;
}
