#ifndef STATES_H
#define STATES_H

#define NUM_STATES 6
#define NUM_ROADS 4

#include "traffic_light.h"

colour states[NUM_STATES][NUM_ROADS] = {
    {RED, GREEN, RED, GREEN},         {RED, AMBER, RED, AMBER},
    {RED_AMBER, RED, RED_AMBER, RED}, {GREEN, RED, GREEN, RED},
    {AMBER, RED, AMBER, RED},         {RED, RED_AMBER, RED, RED_AMBER},
};

#endif  // STATES_H
