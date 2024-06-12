CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
emulate: emulate.o

clean:
	$(RM) *.o assemble emulate

emulate2:
	gcc -Wall emulator/*.c -o emulate
	cp emulate ../../armv8_testsuite/solution/

assemble2:
	gcc -Wall assembler/*.c -o assemble -ggdb3
	cp assemble ../../armv8_testsuite/solution/

assemble_with_valgrind:
	gcc -Wall assembler/*.c -o assemble -ggdb3
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=assemble.txt \
         ./assemble exampleParam1
	cp assemble ../../armv8_testsuite/solution/
