#include "road.h"

// check lights (just once)
// update distance of all cars (starting with first)
// if distance stays the same, add time to stationary time
 // update_distances(road, delta_t); // let cars roll forward if possible (note special case for first car)
void *update_distances(road* update_road, time_t dt)
{
    // the lights can be checked using the road (it has a light attribute)
    if (update_road->light.clr == RED) {

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