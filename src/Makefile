CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
emulate: emulate.o

solution:
	mkdir -p ../armv8_testsuite/solution/

clean:
	$(RM) *.o assemble emulate

emulate2: solution
	gcc -Wall emulator/*.c -o emulate -g --debug
	cp emulate ../armv8_testsuite/solution/

assemble2: solution
	gcc -Wall assembler/*.c -o assemble -g --debug
	cp assemble ../armv8_testsuite/solution/

assemble_with_valgrind:
	gcc -Wall assembler/*.c -o assemble -ggdb3
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=assemble.txt \
         ./assemble ../armv8_testsuite/test/test_cases/general/example.s test.bin

test: assemble2
	cd ../armv8_testsuite && ./run -Apf

led_blink: assemble2
	./assemble ../programs/led_blink.s ../programs/kernel8.img

format:
	clang-format -i assembler/*.c assembler/*.h emulator/*.c emulator/*.h
