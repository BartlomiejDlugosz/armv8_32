#include "states.h"

colour states[NUM_STATES][NUM_ROADS] = {
        {RED, GREEN, RED, GREEN},         {RED, AMBER, RED, AMBER},
        {RED_AMBER, RED, RED_AMBER, RED}, {GREEN, RED, GREEN, RED},
        {AMBER, RED, AMBER, RED},         {RED, RED_AMBER, RED, RED_AMBER},
};