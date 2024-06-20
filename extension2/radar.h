#ifdef RPI

#ifndef RADAR_H
#define RADAR_H

#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>

#define TRIG_PIN 22
#define ECHO_PIN 27

#define RISING_EDGE_LEVEL 1
#define FALLING_EDGE_LEVEL 0
#define PULSE_TIME 10
#define DELAY_PER_ITER 100
#define DISTANCE_SF (0.034 * 0.5)

double get_radar();
void init_radar();

#endif // RADAR_H
#endif // RPI