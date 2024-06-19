#ifdef RPI

#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>

#define TRIG_PIN 22
#define ECHO_PIN 27

struct time_change {
    uint32_t start_time;
    uint32_t end_time;
};

static void change(int gpio, int level, uint32_t tick, void* timer) {
    struct time_change *change_in_time = (struct time_change *) timer;
    if (gpio == ECHO_PIN) {
        if (level == 1) {
            change_in_time->start_time = tick;
        } else if (level == 0) {
            change_in_time->end_time = tick;
        }
    }
}

double get_radar() {
    gpioWrite(TRIG_PIN, 0);

    gpioTrigger(TRIG_PIN, 10, 1);

    struct time_change change_in_time = {0};

    gpioSetAlertFuncEx(ECHO_PIN, &change, &change_in_time);

    while (change_in_time.end_time == 0) {
        gpioDelay(100);
    }

    gpioSetAlertFunc(ECHO_PIN, NULL);

    int difference = change_in_time.end_time - change_in_time.start_time;
    double distance = ((double)difference) * ((double)0.018);

    return distance;
}

void init_radar() {
    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT); 
}

/*
int main() {
    gpioInitialise();

    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT); 

    for (int i = 0; i < 10; i++) {
        printf("%lf\n", get_radar());
        gpioSleep(PI_TIME_RELATIVE, 1, 0);
    }

    gpioTerminate();
    return 0;
}
*/

#endif // RPI
