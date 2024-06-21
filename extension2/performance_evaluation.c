#include <stdio.h>

#include "performance_evaluation.h"
#include "car.h"

// Evaluate a road by calculating average times for
// stationary cars and cars crossed
// Recursive function
void evaluate_road(car *car_crossed, intersection_evaluation *isec_eval,
                   int road_num) {
    if (car_crossed == NULL) {
        return;
    }

    isec_eval->road_evals[road_num]->total_time_stationary +=
        car_crossed->time_stationary;
    isec_eval->road_evals[road_num]->num_cars_crossed++;

    // If a car has waited longer than the current longest waiting time
    // then update the current longest waiting time
    if (car_crossed->time_stationary >
        isec_eval->road_evals[road_num]->maximum_time_stationary) {
        isec_eval->road_evals[road_num]->maximum_time_stationary =
            car_crossed->time_stationary;
    }

    // Recurse and move onto next car
    evaluate_road(car_crossed->next, isec_eval, road_num);
}

// Evaluates all the roads in a intersection
void evaluate_intersection(intersection_evaluation *isec_eval) {
    double average_times_stationary[NUM_ROADS];

    for (int i = 0; i < NUM_ROADS; i++) {
        average_times_stationary[i] =
            (double)isec_eval->road_evals[i]->total_time_stationary /
            (double)isec_eval->road_evals[i]->num_cars_crossed;

        // Updates the longest time stationary throughout the entire intersection
        if (isec_eval->road_evals[i]->maximum_time_stationary >
            isec_eval->total_maximum_time_stationary) {
            isec_eval->total_maximum_time_stationary =
                isec_eval->road_evals[i]->maximum_time_stationary;
        }
        isec_eval->total_average_time_stationary += average_times_stationary[i];
    }

    // Calculate the average time stationary
    isec_eval->total_average_time_stationary =
        isec_eval->total_average_time_stationary / NUM_ROADS;

}