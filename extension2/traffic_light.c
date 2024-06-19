#include <stdio.h>
#include "traffic_light.h"

void print_traffic_light(traffic_light *light) {
    switch(light->clr) {
        case 0:
            printf("RED ");
            break;
        case 1:
            printf("RED_AMBER ");
            break;
        case 2:
            printf("GREEN ");
            break;
        case 3:
            printf("AMBER ");
            break;
    }
}
