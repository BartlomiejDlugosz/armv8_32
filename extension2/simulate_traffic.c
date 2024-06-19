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
#include "show_leds.h"

#ifdef RPI
bool rpi = true;
#else
bool rpi = false;
#endif // RPI

#define N 4
#define MAX_ITERATIONS 10000

#define ROAD0_LENGTH 5000
#define ROAD1_LENGTH 7200
#define ROAD2_LENGTH 6000
#define ROAD3_LENGTH 6500

#define ROAD0_SPEED_LIMIT 60
#define ROAD1_SPEED_LIMIT 75
#define ROAD2_SPEED_LIMIT 50
#define ROAD3_SPEED_LIMIT 65

#define FOLLOW_DIST_SF 15
#define ROAD0_FOLLOW_DIST (int)(ROAD0_SPEED_LIMIT / FOLLOW_DIST_SF)
#define ROAD1_FOLLOW_DIST (int)(ROAD1_SPEED_LIMIT / FOLLOW_DIST_SF)
#define ROAD2_FOLLOW_DIST (int)(ROAD2_SPEED_LIMIT / FOLLOW_DIST_SF)
#define ROAD3_FOLLOW_DIST (int)(ROAD3_SPEED_LIMIT / FOLLOW_DIST_SF)



int main(int argc, char **argv) {
    if (rpi) {
        init_leds();
    }

    srand(time(NULL));   // Initialization, should only be called once.

    // initialise all the structures
    traffic_light light0 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    traffic_light light1 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};
    traffic_light light2 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    traffic_light light3 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};

    road road0 = {.length = ROAD0_LENGTH, .speed_limit = ROAD0_SPEED_LIMIT, .follow_distance = ROAD0_FOLLOW_DIST , .head_car = NULL, .light = &light0 };
    road road1 = {.length = ROAD1_LENGTH, .speed_limit = ROAD1_SPEED_LIMIT, .follow_distance = ROAD1_FOLLOW_DIST , .head_car = NULL, .light = &light1 };
    road road2 = {.length = ROAD2_LENGTH, .speed_limit = ROAD2_SPEED_LIMIT, .follow_distance = ROAD2_FOLLOW_DIST , .head_car = NULL, .light = &light2 };
    road road3 = {.length = ROAD3_LENGTH, .speed_limit = ROAD3_SPEED_LIMIT, .follow_distance = ROAD3_FOLLOW_DIST , .head_car = NULL, .light = &light3 };
    
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

    for (uint64_t iter = 0; iter < MAX_ITERATIONS; iter++) { // timestep
        if (iter % 100 == 0) {
            printf("\n\n\n\nSTART OF ITERATION MOD 100\n");
            print_intersection(isec);
        }
        if (rpi) {
            update_leds(isec->state_index);
        }

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

        if (rpi) {
            sleep(dt);
        }
    }

    for (int i = 0; i < NUM_ROADS; i++) {
        free_all_cars(isec->roads[i]->head_car);
    }
    printf("\ngpio %i\n", rpi);
    if (rpi) {
        terminate_gpio();
    }
    
    return 0;
}
