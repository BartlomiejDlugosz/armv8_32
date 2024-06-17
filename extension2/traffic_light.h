#include <stdbool.h>

typedef enum colour { RED, RED_AMBER, GREEN, AMBER};
typedef enum direction { LEFT, RIGHT };

typedef struct {
    colour clr;
    bool has_arrow;
    bool arrow_lit;
    direction arrow;
    bool sensor; // tells us if cars are waiting
} light;

void update_lights_to_next_state(traffic_light *light);
