#ifdef RPI

#ifndef RADAR_H
#define RADAR_H

#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>

#define TRIG_PIN 22
#define ECHO_PIN 27

double get_radar();
void init_radar();

#endif // RADAR_H
#endif // RPI