#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "strategies.h"
#include "traffic_light.h"
#include "intersection.h"
#include "road.h"
#include "car.h"
#include "states.h"

int main(int argc, char **argv) {

    srand(time(NULL));   // Initialization, should only be called once.

    // initialise all the structures
    traffic_light light0 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    traffic_light light1 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};
    traffic_light light2 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    traffic_light light3 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};

    road road0 = {.length = 10, .speed_limit = 60, .head_car = NULL, .light = light0 };
    road road1 = {.length = 100, .speed_limit = 50, .head_car = NULL, .light = light1 };
    road road2 = {.length = 20, .speed_limit = 10, .head_car = NULL, .light = light2 };
    road road3 = {.length = 100, .speed_limit = 50, .head_car = NULL, .light = light3 };
    
    intersection isec;
    isec.roads[0] = road0;
    isec.roads[1] = road1;
    isec.roads[2] = road2;
    isec.roads[3] = road3;
    isec.state_index = 0;

    //strategy s = &minimax;
    //
    road current_road;
    car *head_of_crossed;
    time_t time_since_change = 0.0;
    time_t dt = 0.5; // seconds
    bool updated;
    uint64_t max_iterations = 100;

    uint64_t i = 0;
    while (i < max_iterations) { // timestep

        //update_lights_to_next_state(isec, dt, &time_since_change); // takes a strategy

        for (int i = 0; i < NUM_ROADS; i++) {
            current_road = isec.roads[i];

            // update_distances(currrent_road, dt); // let cars roll forward if possible (note special case for first car)
            // head_of_crossed = remove_crossed(current_road); // pop off ANY cars which have passed stop line. return the number of cars that crossed
            if (rand() < (RAND_MAX+1u) / N) {// perform with probability 1/N
                //maybe_add_cars(current_road) // also checks sum < length of road
            }
        }

        i++;
    }
    return 0;
}
