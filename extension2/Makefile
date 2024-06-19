CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: simulate_traffic

clean:
	$(RM) *.o simulate_traffic

simulate_traffic: clean
	gcc -Wall car.c intersection.c performance_evaluation.c radar.c road.c show_leds.c simulate_traffic.c states.c strategies.c traffic_light.c update_lights.c -o simulate_traffic -lm -g --debug

simulate_traffic_rpi: clean
	gcc -Wall *.c -o simulate_traffic -lm -lpigpio -lrt -g --debug -D RPI
