#ifdef RPI

#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include "show_leds.h"

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

struct physical_light {
    int pin_num;
    bool state;
};

int light_1_pins[] = {LIGHT_1_RED, LIGHT_1_AMBER, LIGHT_1_GREEN};
int light_2_pins[] = {LIGHT_2_RED, LIGHT_2_AMBER, LIGHT_2_GREEN};
int light_3_pins[] = {LIGHT_3_RED, LIGHT_3_AMBER, LIGHT_3_GREEN};
int light_4_pins[] = {LIGHT_4_RED, LIGHT_4_AMBER, LIGHT_4_GREEN};

struct physical_light all_lights[12];

static void set_light(int pins[], colour clr) {
    switch(clr) {
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
            fprintf(stderr, "UNKNOWN COLOR");
            exit(1);
    }
}

void sleep(int seconds) {
    gpioSleep(PI_TIME_RELATIVE, seconds, 0);
}

void update_leds(int state_index) {
    colour *current_lights = states[state_index];
    set_light(light_1_pins, current_lights[0]);
    set_light(light_2_pins, current_lights[1]);
    set_light(light_3_pins, current_lights[2]);
    set_light(light_4_pins, current_lights[3]);
}

void init_leds() {
    gpioInitialise();

    for (int i = 0; i < 3; i++) {
        all_lights[0 + i] = (struct physical_light) {.pin_num = light_1_pins[i], .state = 0};
    }
    for (int i = 0; i < 3; i++) {
        all_lights[3 + i] = (struct physical_light) {.pin_num = light_2_pins[i], .state = 0};
    }
    for (int i = 0; i < 3; i++) {
        all_lights[6 + i] = (struct physical_light) {.pin_num = light_3_pins[i], .state = 0};    
    }
    for (int i = 0; i < 3; i++) {
        all_lights[9 + i] = (struct physical_light) {.pin_num = light_4_pins[i], .state = 0};
    }

    for (int i = 0; i < 12; i++) {
        gpioSetMode(all_lights[i].pin_num, PI_OUTPUT);
    }
}

void terminate_gpio() {
    gpioTerminate();
}



//int main() {
//    printf("test");
//    init_leds();
    
//    gpioSleep(PI_TIME_RELATIVE, 1, 0);

//    for (int j = 0; j < 10; j++) {
//        for (int i = 0; i < NUM_STATES; i++) {
//            update_leds(i);
//            gpioSleep(PI_TIME_RELATIVE, 1, 0);
//        }
//    }


//    terminate_gpio();

//    return 0;
//}

#endif // RPI
