#ifndef SHOW_LEDS_H
#define SHOW_LEDS_H

#include "states.h"
#include "traffic_light.h"

void update_leds(int state_index);
void sleep(int seconds);
void init_leds(void);
void terminate_gpio(void);


#endif // SHOW_LEDS_H
