#include <stdio.h>
#include "performance_evaluation.h"
#include "car.h"
void evaluate_road(car *car_crossed, intersection_evaluation *isec_eval, int road_num) {
    // printf("STARTED EVALUATING ROAD");
    // print_car(car_crossed);
    // printf("\n\n");
    if (car_crossed == NULL)
    {
        return;
    }
    isec_eval->road_evals[road_num]->total_time_stationary += car_crossed->time_stationary;
    isec_eval->road_evals[road_num]->num_cars_crossed++;
    if (car_crossed->time_stationary > isec_eval->road_evals[road_num]->maximum_time_stationary) {
        isec_eval->road_evals[road_num]->maximum_time_stationary = car_crossed->time_stationary;
    }
    evaluate_road(car_crossed->next, isec_eval, road_num);
}

void evaluate_intersection(intersection_evaluation *isec_eval) {
    double average_times_stationary[NUM_ROADS];
    for (int i = 0; i < NUM_ROADS; i++) {
        average_times_stationary[i] = (double) isec_eval->road_evals[i]->total_time_stationary / (double) isec_eval->road_evals[i]->num_cars_crossed;
        // printf("Road %d, average time stationary %lf, longest waiting time %ld\n", i, average_times_stationary[i], isec_eval->road_evals[i]->maximum_time_stationary);
        if (isec_eval->road_evals[i]->maximum_time_stationary > isec_eval->total_maximum_time_stationary) {
            isec_eval->total_maximum_time_stationary = isec_eval->road_evals[i]->maximum_time_stationary;
        }
        isec_eval->total_average_time_stationary += average_times_stationary[i];
    }
    isec_eval->total_average_time_stationary = isec_eval->total_average_time_stationary / NUM_ROADS;
    // printf("\nAverage across roads %lf, longest waiting time %ld\n", isec_eval->total_average_time_stationary, isec_eval->total_maximum_time_stationary);
}