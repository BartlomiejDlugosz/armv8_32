#include "traffic_light.h"
#include "car.h"

typedef struct {
    int max_length, // traverse the ll, only adding if sum < max_length
    int speed_limit,
    car *first_car,
    traffic_light *light
} road;

// think about case where road->first_car == NULL
// in the case where nothing is updated, the previous
// head is returned so use as
// road->first_car = maybe_add_cars(road, f(x));
car *maybe_add_cars(road, random_function);


/*
 change_function = &minimax;
while (true) { // timestep
    update_lights_to_next_state(change_function) // hmm, is it time to update? minimax

    for (each road) {
        update_distances() // pop off ANY cars which have passed stop line
        remove_crossed() // pop off ANY cars which have passed stop line
        maybe_add_cars() // random function + also check sum < max_length
    }
}

*/

