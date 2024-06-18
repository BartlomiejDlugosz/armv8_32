CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: simulate_traffic

clean:
	$(RM) *.o simulate_traffic

simulate_traffic: solution
	gcc -Wall ../extension2/*.c -o simulate_traffic -g --debug
