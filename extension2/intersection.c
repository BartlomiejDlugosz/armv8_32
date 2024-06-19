#include <stdio.h>
#include "intersection.h"
#include "states.h"

void print_intersection(intersection *isec) {
    for(int i = 0; i < NUM_ROADS; i++) {
        print_road(&isec->roads[i]);
    }
    printf("\n\n");
}