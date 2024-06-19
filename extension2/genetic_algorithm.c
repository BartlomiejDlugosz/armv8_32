#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "intersection.h"
#include "strategies.h"
#include "states.h"
#include "performance_evaluation.h"

#define POP_SIZE 50
#define NUM_STATES 6
#define MAX_GENERATIONS 20
#define MUTATION_RATE 0.1
#define CROSSOVER_RATE 0.7
#define MAX_DURATION 100
#define NUM_ROADS 4

typedef struct {
    int durations[NUM_STATES];
    double fitness;
} Chromosome;

Chromosome population[POP_SIZE];

// void initialize_population();
// double evaluate_fitness(Chromosome *chromo, intersection *isec, int car_counts[], double road_speeds[], strategy s);
// void evaluate_population(intersection *isec, int car_counts[], double road_speeds[], strategy s);
// void select_parents(Chromosome *parent1, Chromosome *parent2);
// void crossover(Chromosome *parent1, Chromosome *parent2, Chromosome *child1, Chromosome *child2);
// void mutate(Chromosome *chromo);
// void create_new_population(intersection *isec, int car_counts[], double road_speeds[], strategy s);
// Chromosome get_best_chromosome();

void initialize_population() {
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < NUM_STATES; j++) {
            population[i].durations[j] = rand() % MAX_DURATION + 1; // Random duration between 1 and MAX_DURATION
        }
    }
}
// to evaluate fitness it needs to run the chromosome through simulate traffic 10 times then calculate the average fitness
double evaluate_fitness(intersection_evaluation *isec_eval, bool is_avg) {
    if (is_avg) {
        return isec_eval->total_average_time_stationary;
    } else {
        return isec_eval->total_maximum_time_stationary;
    }
}

// this evaluates each chromosome
void evaluate_population(Chromosome *population[], bool is_avg) {
    for (int i = 0; i < POP_SIZE; i++) {
        population[i]->fitness = evaluate_fitness(&population[i], is_avg);
    }
}

// this selects the new parents to creaete a new generation
void select_parents(Chromosome *parent1, Chromosome *parent2) {
    int idx1 = rand() % POP_SIZE;
    int idx2 = rand() % POP_SIZE;
    *parent1 = population[idx1];
    *parent2 = population[idx2];
}

//this randomly crosses over the durations
void crossover(Chromosome *parent1, Chromosome *parent2, Chromosome *child1, Chromosome *child2) {
    if (((double)rand() / RAND_MAX) < CROSSOVER_RATE) {
        int crossover_point = rand() % NUM_STATES;
        for (int i = 0; i < crossover_point; i++) {
            child1->durations[i] = parent1->durations[i];
            child2->durations[i] = parent2->durations[i];
        }
        for (int i = crossover_point; i < NUM_STATES; i++) {
            child1->durations[i] = parent2->durations[i];
            child2->durations[i] = parent1->durations[i];
        }
    } else {
        *child1 = *parent1;
        *child2 = *parent2;
    }
}

void mutate(Chromosome *chromo) {
    for (int i = 0; i < NUM_STATES; i++) {
        if (((double)rand() / RAND_MAX) < MUTATION_RATE) {
            chromo->durations[i] = rand() % MAX_DURATION + 1; // Random mutation
        }
    }
}

void create_new_population(intersection *isec, int car_counts[], double road_speeds[], strategy s) {
    Chromosome new_population[POP_SIZE];
    for (int i = 0; i < POP_SIZE / 2; i++) {
        Chromosome parent1, parent2, child1, child2;
        select_parents(&parent1, &parent2);
        crossover(&parent1, &parent2, &child1, &child2);
        mutate(&child1);
        mutate(&child2);
        new_population[2 * i] = child1;
        new_population[2 * i + 1] = child2;
    }
    for (int i = 0; i < POP_SIZE; i++) {
        population[i] = new_population[i];
    }
}

Chromosome get_best_chromosome() {
    Chromosome best = population[0];
    for (int i = 1; i < POP_SIZE; i++) {
        if (population[i].fitness > best.fitness) {
            best = population[i];
        }
    }
    return best;
}

// int main() {
//     srand(time(NULL));

//     // Define the car counts and road speeds for the 4 roads
//     int car_counts[NUM_ROADS] = {30, 20, 50, 10};
//     double road_speeds[NUM_ROADS] = {30.0, 40.0, 20.0, 35.0};

//     intersection isec;
//     isec.state_index = 0;
//     for (int i = 0; i < NUM_ROADS; i++) {
//         isec->roads[i]->light->clr = states[0][i];
//     }

//     initialize_population();
//     evaluate_population(&isec, car_counts, road_speeds, basic);

//     for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
//         create_new_population(&isec, car_counts, road_speeds, basic);
//         evaluate_population(&isec, car_counts, road_speeds, basic);
        
//         Chromosome best = get_best_chromosome();
//         printf("Generation %d: Best Fitness = %f\n", generation, best.fitness);
//     }

//     Chromosome best = get_best_chromosome();
//     printf("Best solution found: \n");
//     for (int i = 0; i < NUM_STATES; i++) {
//         printf("%d ", best.durations[i]);
//     }
//     printf("\nFitness: %f\n", best.fitness);

//     return 0;
// }