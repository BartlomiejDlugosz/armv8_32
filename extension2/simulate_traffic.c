#include "strategies.h"
#include "traffic_light.h"
#include "road.h"
#include "car.h"

int main(int argc, char **argv) {

    double time_since_change = 0.0;
    double delta_t = 0.5; // seconds

    //strategy s = &minimax;
    while (true) { // timestep

        update_lights_to_next_state(); // takes a strategy

        for (int i = 0; i < 4; i++) {
            update_distances(); // let cars roll forward if possible (note special case for first car)
            remove_crossed(); // pop off ANY cars which have passed stop line
            maybe_add_cars(); // random function + also check sum < max_length
        }
    }

    return 0;
}
