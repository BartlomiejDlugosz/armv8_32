#ifdef RPI

#include "show_leds.h"

int light_1_pins[] = {LIGHT_1_RED, LIGHT_1_AMBER, LIGHT_1_GREEN};
int light_2_pins[] = {LIGHT_2_RED, LIGHT_2_AMBER, LIGHT_2_GREEN};
int light_3_pins[] = {LIGHT_3_RED, LIGHT_3_AMBER, LIGHT_3_GREEN};
int light_4_pins[] = {LIGHT_4_RED, LIGHT_4_AMBER, LIGHT_4_GREEN};

// This sets a light to the specified color
// Pre: len(pins) == 3
static void set_light(int pins[], colour clr) {
    switch (clr) {
        case RED:
            gpioWrite(pins[0], 1);
            gpioWrite(pins[1], 0);
            gpioWrite(pins[2], 0);
            break;
        case AMBER:
            gpioWrite(pins[0], 0);
            gpioWrite(pins[1], 1);
            gpioWrite(pins[2], 0);
            break;
        case RED_AMBER:
            gpioWrite(pins[0], 1);
            gpioWrite(pins[1], 1);
            gpioWrite(pins[2], 0);
            break;
        case GREEN:
            gpioWrite(pins[0], 0);
            gpioWrite(pins[1], 0);
            gpioWrite(pins[2], 1);
            break;
        default:
            fprintf(stderr, "UNKNOWN COLOUR %i\n", clr);
            exit(1);
    }
}

// Initialises/Terminates the gpio
void init_gpio(void) { gpioInitialise(); }

void terminate_gpio(void) { gpioTerminate(); }

// Function to sleep to be used from other files which don't include pigpio
void sleep(int seconds) { gpioSleep(PI_TIME_RELATIVE, seconds, 0); }

// This updates all the lights to the color they should be according to
// the current state index
void update_leds(int state_index) {
    colour *current_lights = states[state_index];
    set_light(light_1_pins, current_lights[0]);
    set_light(light_2_pins, current_lights[1]);
    set_light(light_3_pins, current_lights[2]);
    set_light(light_4_pins, current_lights[3]);
}

// Sets the pin mode for all leds to output
void init_leds(void) {
    for (int i = 0; i < PINS_PER_LIGHT; i++) {
        gpioSetMode(light_1_pins[i], PI_OUTPUT);
    }
    for (int i = 0; i < PINS_PER_LIGHT; i++) {
        gpioSetMode(light_2_pins[i], PI_OUTPUT);
    }
    for (int i = 0; i < PINS_PER_LIGHT; i++) {
        gpioSetMode(light_3_pins[i], PI_OUTPUT);
    }
    for (int i = 0; i < PINS_PER_LIGHT; i++) {
        gpioSetMode(light_4_pins[i], PI_OUTPUT);
    }
}

#endif  // RPI
