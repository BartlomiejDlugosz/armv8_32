#ifdef RPI

#include "radar.h"

// Structure that captures the start and end time of the echo_pin
// used in calculating the distance
struct time_change {
    uint32_t start_time;
    uint32_t end_time;
};

// Initialise ultrasonic sensors pins
void init_radar() {
    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);
}

// Used as a callback function when echo pin changes state
static void updateTimer(int gpio, int level, uint32_t tick, void* timer) {
    struct time_change *change_in_time = (struct time_change *) timer;
    // Make sure we are handling the echo pin
    if (gpio == ECHO_PIN) {
        if (level == RISING_EDGE_LEVEL) {
            // If a rising edge then set the start to the current tick (microseconds)
            change_in_time->start_time = tick;
        } else if (level == FALLING_EDGE_LEVEL) {
            // If falling edge then set end time to the current tick (microseconds)
            change_in_time->end_time = tick;
        }
    }
}

// Reads the distance from the ultrasonic sensor
double get_radar() {
    // Set trigger pin to low to ensure it's off
    gpioWrite(TRIG_PIN, 0);

    // Send a pulse to send ultrasonic beam
    gpioTrigger(TRIG_PIN, PULSE_TIME, 1);

    // Initialise a new structure which will contain the start and end times
    struct time_change change_in_time = {0};

    // Add a callback to the echo pin which will call the function when a change
    // in state is detected
    gpioSetAlertFuncEx(ECHO_PIN, &updateTimer, &change_in_time);

    // Wait for the appropriate amount of time
    while (change_in_time.end_time == 0) {
        gpioDelay(DELAY_PER_ITER);
    }

    // Remove callback
    gpioSetAlertFunc(ECHO_PIN, NULL);

    // Calculate the difference in time and calculate the distance
    int difference = change_in_time.end_time - change_in_time.start_time;
    double distance = difference * DISTANCE_SF;

    return distance;
}

#endif // RPI
