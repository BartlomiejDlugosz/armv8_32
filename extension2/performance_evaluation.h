#ifndef PERFORMANCE_EVALUATION_H
#define PERFORMANCE_EVALUATION_H
#include <time.h>
#include "states.h"
#include "car.h"
typedef struct {
    time_t total_time_stationary;
    int num_cars_crossed;
    time_t maximum_time_stationary;
} road_evaluation;

typedef struct {
    road_evaluation *road_evals[NUM_ROADS];
    double total_average_time_stationary;
    time_t total_maximum_time_stationary;
} intersection_evaluation;

void evaluate_road(car *car_crossed, intersection_evaluation *isec_eval, int road_num);

void evaluate_intersection(intersection_evaluation *isec_eval);

#endif // PERFORMANCE_EVALUATION_H