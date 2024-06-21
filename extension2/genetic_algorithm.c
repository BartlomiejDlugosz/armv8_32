#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strategies.h"
#include "performance_evaluation.h"
#include "simulate_traffic.h"
#include "genetic_algorithm.h"

#define POP_SIZE 100
#define NUM_STATES 6
#define MAX_GENERATIONS 10
#define MUTATION_RATE 0.1
#define CROSSOVER_RATE 0.7
#define MAX_DURATION 50
#define NUM_ROADS 4
#define CHROMOSOME_EVAL_NUM 5
#define TRAINING "training"

// this selects the best data set from a population
static Chromosome get_best_chromosome(Chromosome *population) {
    Chromosome best = population[0];
    for (int i = 1; i < POP_SIZE; i++) {
        if (population[i].fitness < best.fitness) {
            best = population[i];
        }
    }
    return best;
}

static void print_chromosome(Chromosome *chromo, bool is_avg) {
    if (is_avg) {
        printf("Best chromosome for minimising average waiting time per car\n");
    } else {
        printf("Best chromosome for minimising maximum waiting time per car\n");
    }
    for (int i = 0; i < NUM_STATES; i++) {
        printf("state %i, duration %i\n", i, chromo->durations[i]);
    }
    printf("\n\nfitness %lf\n", chromo->fitness);
}

// to evaluate fitness it needs to run the chromosome through simulate traffic
// 10 times then calculate the average fitness
static void evaluate_fitness(Chromosome *chromo, bool is_avg) {
    for (int i = 0; i < CHROMOSOME_EVAL_NUM; i++) {
        intersection_evaluation *isec_eval =
            simulate_traffic(genetic_algorithm, chromo, TRAINING);
        if (isec_eval == NULL) {
            // Failed to allocate memory
            // Undefined behaviour here so just return
            fprintf(stderr, "Failed to simulate traffic");
            return;
        }
        if (is_avg) {
            chromo->fitness += isec_eval->total_average_time_stationary;
        } else {
            chromo->fitness += (double)isec_eval->total_maximum_time_stationary;
        }
        free_isec_eval(isec_eval);
    }
    chromo->fitness = chromo->fitness / CHROMOSOME_EVAL_NUM;
}

// this selects the new parents to creaete a new generation
static void select_parents(Chromosome *population, Chromosome *parent1,
                           Chromosome *parent2) {
    int id1 = rand() % POP_SIZE;
    int id2 = rand() % POP_SIZE;
    bool take_best = rand() % 2;
    *parent1 = population[id1];
    *parent2 = population[id2];
    if (take_best) {
        *parent1 = get_best_chromosome(population);
    }
}

// this randomly crosses over the durations for the two parents to the two
// children
static void crossover(Chromosome *parent1, Chromosome *parent2,
                      Chromosome *child1, Chromosome *child2) {
    if (((double)rand() / RAND_MAX) < CROSSOVER_RATE) {
        int crossover_point = rand() % NUM_STATES;
        for (int i = 0; i < crossover_point; i++) {
            child1->durations[i] = 0;
            child2->durations[i] = 0;
            child1->durations[i] = parent1->durations[i];
            child2->durations[i] = parent2->durations[i];
        }
        for (int i = crossover_point; i < NUM_STATES; i++) {
            child1->durations[i] = 0;
            child2->durations[i] = 0;
            child1->durations[i] = parent2->durations[i];
            child2->durations[i] = parent1->durations[i];
        }
    } else {
        *child1 = *parent1;
        *child2 = *parent2;
    }
    child1->fitness = 0;
    child2->fitness = 0;
}

// randomly mutatues a chromosome
static void mutate(Chromosome *chromo) {
    for (int i = 0; i < NUM_STATES; i++) {
        if (((double)rand() / RAND_MAX) < MUTATION_RATE) {
            chromo->durations[i] = 0;
            chromo->durations[i] =
                (rand() % MAX_DURATION) + 1;  // Random mutation
        }
    }
}

// creates a new population given two parents and 2 children
static void create_new_population(Chromosome *population) {
    Chromosome new_population[POP_SIZE];
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < NUM_STATES; j++) {
            new_population[i].durations[j] = 0;
        }
        new_population[i].fitness = 0;
    }
    for (int i = 0; i < POP_SIZE / 2; i++) {
        Chromosome parent1, parent2, child1, child2;
        select_parents(population, &parent1, &parent2);
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

Chromosome train_genetic_algorithm(bool is_avg) {
    Chromosome population[POP_SIZE];
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < NUM_STATES; j++) {
            population[i].durations[j] = 0;
            population[i].durations[j] = (rand() % MAX_DURATION) + 1;
        }
        population[i].fitness = 0;
    }
    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        for (int i = 0; i < POP_SIZE; i++) {
            evaluate_fitness(&population[i], is_avg);
        }
        create_new_population(population);
    }
    for (int i = 0; i < POP_SIZE; i++) {
        evaluate_fitness(&population[i], is_avg);
    }
    Chromosome best = get_best_chromosome(population);
    print_chromosome(&best, is_avg);
    return get_best_chromosome(population);
}
