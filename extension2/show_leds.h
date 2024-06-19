#ifdef RPI

#ifndef SHOW_LEDS_H
#define SHOW_LEDS_H

#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>

#include "states.h"
#include "traffic_light.h"

#define PINS_PER_LIGHT 3

// These are the GPIO pin numbers, not physical
#define LIGHT_1_RED 2
#define LIGHT_1_AMBER 3
#define LIGHT_1_GREEN 4

#define LIGHT_2_RED 7
#define LIGHT_2_AMBER 8
#define LIGHT_2_GREEN 25

#define LIGHT_3_RED 21
#define LIGHT_3_AMBER 20
#define LIGHT_3_GREEN 16

#define LIGHT_4_RED 13
#define LIGHT_4_AMBER 6
#define LIGHT_4_GREEN 5

void update_leds(int state_index);
void sleep(int seconds);
void init_leds(void);
void init_gpio(void);
void terminate_gpio(void);


#endif // SHOW_LEDS_H
#endif // RPI
