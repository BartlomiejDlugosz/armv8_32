#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>  // debugging

#include "traffic_light.h"
#include "states.h"
#include "road.h"
#include "intersection.h"
#include "strategies.h"
#include "update_lights.h"
#include "performance_evaluation.h"
#include "simulate_traffic.h"

#ifdef RPI
#include "radar.h"
#include "show_leds.h"
#endif  // RPI

#define N 4
#define DT 1  // seconds
#define INITIAL_STATE_INDEX 0
#define INITIAL_NUM_CARS 0
#define MAX_ITERATIONS 100000
#define MAX_FILE_NAME_SIZE 50
#define PRINT_EVERY_ITER 1000

#define ROAD0_LENGTH 500
#define ROAD1_LENGTH 720
#define ROAD2_LENGTH 600
#define ROAD3_LENGTH 650

#define ROAD0_SPEED_LIMIT 60
#define ROAD1_SPEED_LIMIT 75
#define ROAD2_SPEED_LIMIT 38
#define ROAD3_SPEED_LIMIT 40

#define FOLLOW_DIST_SF 3
#define ROAD0_FOLLOW_DIST (int)(ROAD0_SPEED_LIMIT / FOLLOW_DIST_SF)
#define ROAD1_FOLLOW_DIST (int)(ROAD1_SPEED_LIMIT / FOLLOW_DIST_SF)
#define ROAD2_FOLLOW_DIST (int)(ROAD2_SPEED_LIMIT / FOLLOW_DIST_SF)
#define ROAD3_FOLLOW_DIST (int)(ROAD3_SPEED_LIMIT / FOLLOW_DIST_SF)

// Frees a intersection_evaluation
void free_isec_eval(intersection_evaluation *isec_eval) {
    for (int i = 0; i < NUM_ROADS; i++) {
        free(isec_eval->road_evals[i]);
    }
    free(isec_eval);
}

// Initialises the intersection evaluation by allocating memory for
// each road. Returns true if successfull, false if failed to malloc
bool initialise_isec_eval(intersection_evaluation *isec_eval) {
    for (int i = 0; i < NUM_ROADS; i++) {
        isec_eval->road_evals[i] = malloc(sizeof(road_evaluation));

        // Free entire isec_eval and exit since malloc failed
        if (isec_eval->road_evals[i] == NULL) {
            free_isec_eval(isec_eval);

            fprintf(stderr, "Failed to allocate memory for roads");
            return false;
        }

        // Initialize to 0, else causes bugs on different systems
        memset(isec_eval->road_evals[i], 0, sizeof(road_evaluation));
    }
    isec_eval->total_average_time_stationary = 0;
    isec_eval->total_maximum_time_stationary = 0;
    return true;
}

intersection_evaluation *simulate_traffic(strategy target_strategy,
                                          Chromosome *optimal_data,
                                          char *strategy_name) {
#ifdef RPI
    init_gpio();
    init_leds();
    init_radar();

    // Add the sensor to trafic light 0 if the raspberry pi
    // is connected
    traffic_light light0 = {.clr = RED,
                            .has_arrow = false,
                            .has_sensor = true,
                            .sensor_distance = get_radar()};
#else   // RPI
    traffic_light light0 = {
        .clr = RED, .has_arrow = false, .has_sensor = false};
#endif  // RPI

    traffic_light light1 = {
        .clr = GREEN, .has_arrow = false, .has_sensor = false};
    traffic_light light2 = {
        .clr = RED, .has_arrow = false, .has_sensor = false};
    traffic_light light3 = {
        .clr = GREEN, .has_arrow = false, .has_sensor = false};

    road road0 = {.length = ROAD0_LENGTH,
                  .speed_limit = ROAD0_SPEED_LIMIT,
                  .follow_distance = ROAD0_FOLLOW_DIST,
                  .head_car = NULL,
                  .light = &light0,
                  .num_cars = INITIAL_NUM_CARS};
    road road1 = {.length = ROAD1_LENGTH,
                  .speed_limit = ROAD1_SPEED_LIMIT,
                  .follow_distance = ROAD1_FOLLOW_DIST,
                  .head_car = NULL,
                  .light = &light1,
                  .num_cars = INITIAL_NUM_CARS};
    road road2 = {.length = ROAD2_LENGTH,
                  .speed_limit = ROAD2_SPEED_LIMIT,
                  .follow_distance = ROAD2_FOLLOW_DIST,
                  .head_car = NULL,
                  .light = &light2,
                  .num_cars = INITIAL_NUM_CARS};
    road road3 = {.length = ROAD3_LENGTH,
                  .speed_limit = ROAD3_SPEED_LIMIT,
                  .follow_distance = ROAD3_FOLLOW_DIST,
                  .head_car = NULL,
                  .light = &light3,
                  .num_cars = INITIAL_NUM_CARS};

    intersection isec = {0};

    isec.roads[0] = &road0;
    isec.roads[1] = &road1;
    isec.roads[2] = &road2;
    isec.roads[3] = &road3;
    isec.state_index = INITIAL_STATE_INDEX;

    // Need to malloc since pointer is returned
    intersection_evaluation *isec_eval =
        malloc(sizeof(intersection_evaluation));

    // Return NULL if failed to allocate memory anywhere since
    // we can't do anything and need to ensure all files get closed
    if (isec_eval == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for intersection evaluation");
        return NULL;
    }

    if (!initialise_isec_eval(isec_eval)) {
        return NULL;
    }

    // strategy target_strategy = basic_plus;

    time_t time_since_change = 0;

    // Strategy names no longer than 25 characters
    char fname[MAX_FILE_NAME_SIZE];
    FILE *files[NUM_ROADS];

    for (int i = 0; i < NUM_ROADS; i++) {
        sprintf(fname, "./graphing/%s-road%i.txt", strategy_name, i);
        files[i] = fopen(fname, "w");
    }

    for (uint64_t iter = 0; iter < MAX_ITERATIONS; iter++) {  // timestep
        // Every number of iterations write the number of cars on the road
        if (iter % PRINT_EVERY_ITER == 0) {
            for (int i = 0; i < NUM_ROADS; i++) {
                fprintf(files[i], "%d,", isec.roads[i]->num_cars);
            }
        }

#ifdef RPI
        // Update the sensor reading
        isec->roads[0]->light->sensor_distance = get_radar();
#endif  // RPI

        // NOTE: also deals with updating physical LEDs
        update_lights_to_next_state(&isec, DT, &time_since_change,
                                    target_strategy,
                                    optimal_data);  // takes a strategy

        for (int i = 0; i < NUM_ROADS; i++) {
            road *current_road = isec.roads[i];

            update_distances(current_road,
                             DT);  // let cars roll forward if possible (note
                                   // special case for first car)
            car *head_of_crossed = remove_crossed(
                current_road);  // pop off ANY cars which have passed stop line.
                                // return the number of cars that crossed
            evaluate_road(head_of_crossed, isec_eval, i);
            free_all_cars(
                head_of_crossed);  // because we don't calculate best algo yet

            if (rand() < (RAND_MAX + 1u) / N) {  // perform with probability 1/N
                maybe_add_car(
                    current_road);  // also checks sum < length of road
            }
        }

#ifdef RPI
        // Sleep for number of seconds
        sleep(DT);
#endif  // RPI
    }
    evaluate_intersection(isec_eval);

    for (int i = 0; i < NUM_ROADS; i++) {
        free_all_cars(isec.roads[i]->head_car);
    }
#ifdef RPI
    // Terminate gpio
    terminate_gpio();
#endif  // RPI

    for (int i = 0; i < NUM_ROADS; i++) {
        fclose(files[i]);
    }

    return isec_eval;
}
