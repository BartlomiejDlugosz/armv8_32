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
#include "performance_evaluation.h"
#include "simulate_traffic.h"

#ifdef RPI
#include "radar.h"
#include "show_leds.h"
#endif // RPI

#define N 4
#define DT 1 // seconds
#define INITIAL_STATE_INDEX 0
#define INITIAL_NUM_CARS 0
#define MAX_ITERATIONS 100000

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



intersection_evaluation* simulate_traffic(strategy s, Chromosome *optimal_data) {
    #ifdef RPI
    init_gpio();
    init_leds();
    init_radar();
    #endif // RPI


    // initialise all the structures
    #ifdef RPI
    traffic_light light0 = {.clr = RED, .has_arrow = false, .has_sensor = true, .sensor_distance = get_radar() };
    #else 
    traffic_light light0 = {.clr = RED, .has_arrow = false, .has_sensor = false };
    #endif

    traffic_light light1 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};
    traffic_light light2 = {.clr = RED, .has_arrow = false, .has_sensor = false};
    traffic_light light3 = {.clr = GREEN, .has_arrow = false, .has_sensor = false};

    road road0 = {.length = ROAD0_LENGTH, .speed_limit = ROAD0_SPEED_LIMIT, .follow_distance = ROAD0_FOLLOW_DIST , .head_car = NULL, .light = &light0, .num_cars = INITIAL_NUM_CARS };
    road road1 = {.length = ROAD1_LENGTH, .speed_limit = ROAD1_SPEED_LIMIT, .follow_distance = ROAD1_FOLLOW_DIST , .head_car = NULL, .light = &light1, .num_cars = INITIAL_NUM_CARS };
    road road2 = {.length = ROAD2_LENGTH, .speed_limit = ROAD2_SPEED_LIMIT, .follow_distance = ROAD2_FOLLOW_DIST , .head_car = NULL, .light = &light2, .num_cars = INITIAL_NUM_CARS };
    road road3 = {.length = ROAD3_LENGTH, .speed_limit = ROAD3_SPEED_LIMIT, .follow_distance = ROAD3_FOLLOW_DIST , .head_car = NULL, .light = &light3, .num_cars = INITIAL_NUM_CARS };
    
    intersection isec_struct;
    intersection *isec;

    isec = &isec_struct;
    isec->roads[0] = &road0;
    isec->roads[1] = &road1;
    isec->roads[2] = &road2;
    isec->roads[3] = &road3;
    isec->state_index = INITIAL_STATE_INDEX;

    road_evaluation road0_eval = {0};
    road_evaluation road1_eval = {0};
    road_evaluation road2_eval = {0};
    road_evaluation road3_eval = {0};
    
//    intersection_evaluation isec_eval_struct;
    intersection_evaluation *isec_eval = malloc(sizeof(intersection_evaluation));

    if (isec_eval == NULL) {
        fprintf(stderr, "Unable to allocate memory");
        exit(1);
    }

    isec_eval->road_evals[0] = &road0_eval;
    isec_eval->road_evals[1] = &road1_eval;
    isec_eval->road_evals[2] = &road2_eval;
    isec_eval->road_evals[3] = &road3_eval;
    isec_eval->total_average_time_stationary = 0;
    isec_eval->total_maximum_time_stationary = 0;

    
    // strategy s = basic_plus;
    
    road *current_road;
    car *head_of_crossed;

    time_t initial_time_since_change = 0;
    time_t *time_since_change;
    time_since_change = &initial_time_since_change;


    FILE *f0 = fopen("./graphing/road0.txt", "w"); 
    FILE *f1 = fopen("./graphing/road1.txt", "w"); 
    FILE *f2 = fopen("./graphing/road2.txt", "w"); 
    FILE *f3 = fopen("./graphing/road3.txt", "w"); 

    for (uint64_t iter = 0; iter < MAX_ITERATIONS; iter++) { // timestep
        // if (iter % 100 == 0) {
        //     printf("\n\n\n\nSTART OF ITERATION MOD 100\n");
        //     print_intersection(isec);
        // }
        if (iter % 1000 == 0) {
            fprintf(f0, "%d,", isec->roads[0]->num_cars);
            fprintf(f1, "%d,", isec->roads[1]->num_cars);
            fprintf(f2, "%d,", isec->roads[2]->num_cars);
            fprintf(f3, "%d,", isec->roads[3]->num_cars);
        }

        #ifdef RPI
        update_leds(isec->state_index);
        isec->roads[0]->light->sensor_distance = get_radar();
        #endif // RPI

        // NOTE: also deals with updating physical LEDs
        update_lights_to_next_state(isec, DT, time_since_change, s, optimal_data); // takes a strategy
        
        for (int i = 0; i < NUM_ROADS; i++) {
            current_road = isec->roads[i];

            update_distances(current_road, DT); // let cars roll forward if possible (note special case for first car)
            head_of_crossed = remove_crossed(current_road); // pop off ANY cars which have passed stop line. return the number of cars that crossed
            evaluate_road(head_of_crossed, isec_eval, i);
            free_all_cars(head_of_crossed); // because we don't calculate best algo yet

            if (rand() < (RAND_MAX+1u) / N) {// perform with probability 1/N
                maybe_add_car(current_road); // also checks sum < length of road
            }
        }

        #ifdef RPI
        sleep(DT);
        #endif // RPI
    }
    evaluate_intersection(isec_eval);
    for (int i = 0; i < NUM_ROADS; i++) {
        free_all_cars(isec->roads[i]->head_car);
    }
    #ifdef RPI
    terminate_gpio();
    #endif // RPI

    fclose(f0);
    fclose(f1);
    fclose(f2);
    fclose(f3);

    return isec_eval;
}
