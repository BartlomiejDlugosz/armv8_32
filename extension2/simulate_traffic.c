#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "strategies.h"
#include "traffic_light.h"
#include "traffic_light.h"
#include "intersection.h"
#include "road.h"
#include "car.h"

#define NUM_STATES 6
#define NUM_ROADS 4

colour states[NUM_STATES][NUM_ROADS] = {
    {RED, GREEN, RED, GREEN},
    {RED, AMBER, RED, AMBER},
    {RED_AMBER, RED, RED_AMBER, RED},
    {GREEN, RED, GREEN, RED},
    {AMBER, RED, AMBER, RED},
    {RED, RED_AMBER, RED, RED_AMBER},
};

int main(int argc, char **argv) {


    // initialise all the structures
    road road0 = {.length = 10, .speed_limit = 60, .head_car = NULL };
    road road1 = {.length = 100, .speed_limit = 50, .head_car = NULL };
    road road2 = {.length = 20, .speed_limit = 10, .head_car = NULL };
    road road3 = {.length = 100, .speed_limit = 50, .head_car = NULL };

    light light0 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    light light1 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};
    light light2 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    light light3 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};
    
    intersection isec;
    isec.roads[0] = road0;
    isec.roads[1] = road1;
    isec.roads[2] = road2;
    isec.roads[3] = road3;
    isec.lights[0] = light0;
    isec.lights[1] = light1;
    isec.lights[2] = light2;
    isec.lights[3] = light3;
    isec.state_index = 0;

    //strategy s = &minimax;
    //
    double time_since_change = 0.0;
    double delta_t = 0.5; // seconds
    bool updated;
    uint64_t max_iterations = 100;

    uint64_t i = 0;
    while (i < max_iterations) { // timestep

        updated = update_lights_to_next_state(); // takes a strategy
        if (updated) {
            time_since_change = 0.0;
        } else {
            time_since_change += delta_t;
        }

        for (int i = 0; i < 4; i++) {
            //update_distances(); // let cars roll forward if possible (note special case for first car)
            // num_crossed = remove_crossed(); // pop off ANY cars which have passed stop line. return the number of cars that crossed
            //maybe_add_cars(); // random function + also check sum < max_length
        }

        i++;
    }

    return 0;
}
