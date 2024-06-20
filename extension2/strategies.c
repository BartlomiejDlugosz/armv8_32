#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "strategies.h"
#include "genetic_algorithm.h"

static bool cars_waiting(intersection *isec) {
    bool cars_waiting_bool = false;
    for(int i = 0;  i < NUM_ROADS; i++) {
        cars_waiting_bool |= (isec->roads[i]->light->clr == RED && isec->roads[i]->head_car != NULL);
    }
    return cars_waiting_bool;
}

static double sigmoid(double sensor_distance) {
    double exponent = (double) -0.5 * sensor_distance + 3;
    return ((double) 1.00) / (double) (1.00 + exp(exponent));
}

static double sensor_significance(intersection *isec) {
    if (isec->roads[0]->light->clr == RED && isec->roads[0]->light->has_sensor) {
        return sigmoid(isec->roads[0]->light->sensor_distance);
    } else {
        return 1;
    }
}

static bool amber_lights(intersection *isec) {
    bool amber_light = false;
    for(int i = 0;  i < NUM_ROADS; i++) {
        amber_light |= (isec->roads[i]->light->clr == RED_AMBER || isec->roads[i]->light->clr == AMBER);
    }
    return amber_light;
}

bool basic (intersection *isec, time_t time_since_change, Chromosome *optimal_data) {
    if (amber_lights(isec) && time_since_change > TIME_TO_CHANGE_AMBER) {
        return true;
    } else if (time_since_change > TIME_TO_CHANGE_BASIC) {
        return true;
    }
    return false;
}

bool basic_plus (intersection *isec, time_t time_since_change, Chromosome *optimal_data) {
    if (amber_lights(isec) && time_since_change > TIME_TO_CHANGE_AMBER) {
        return true;
    } else if (time_since_change > TIME_TO_CHANGE_BASIC * sensor_significance(isec)) {
        return cars_waiting(isec);
    }
    return false;
}

// what i need: duration to hold for each traffic light state where it's green 
bool genetic_algorithm (intersection *isec, time_t time_since_change, Chromosome *optimal_data) {
    if(amber_lights(isec) && time_since_change > TIME_TO_CHANGE_AMBER) {
        return true;
    } else if(!amber_lights(isec) && (optimal_data->durations[isec->state_index] * sensor_significance(isec) < time_since_change)) {
        return true;
    }
    return false;
}

strategy strategies[NUM_STRATEGIES] = {basic, basic_plus, genetic_algorithm};
char* strategy_names[NUM_STRATEGIES] = {"Basic", "Basic Plus", "Genetic Algorithm"};
