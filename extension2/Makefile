CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: simulation

clean:
	$(RM) *.o simulation_traffic

simulation:
	gcc -Wall ./*.c -o simulation -lm -g --debug

simulation_rpi:
	gcc -Wall *.c -o simulation -lm -lpigpio -lrt -g --debug -D RPI

graphs:
	cd graphing/ && python3 main.py
