typedef struct {
    int distance_to_next, // distance_to_light if next == NULL
    car *next, // possibly null
} car;


// check lights (just once)
// update distance of all cars (starting with first)
void *update_distances(road);

// pop off ANY cars which have passed stop line, that involves
// potentially popping many cars, updating the running head
// return head
car *remove_crossed(road);

