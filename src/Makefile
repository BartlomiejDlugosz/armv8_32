CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

solution:
	mkdir -p ../armv8_testsuite/solution/

clean:
	$(RM) *.o assemble emulate

emulate: solution
	gcc -Wall emulator/*.c -o emulate -g --debug
	cp emulate ../armv8_testsuite/solution/

assemble: solution
	gcc -Wall assembler/assemble.c assembler/branch_encoder.c assembler/data_processing_assembly.c assembler/dynamicString.c assembler/encode_instruction.c assembler/parser.c assembler/passing.c assembler/register_to_number.c assembler/single_data_transfer_assembly.c assembler/symbol_table.c -o assemble -g --debug
	cp assemble ../armv8_testsuite/solution/

assemble_with_valgrind:
	gcc -Wall assembler/*.c -o assemble -ggdb3
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=assemble.txt \
         ./assemble ../armv8_testsuite/test/test_cases/general/example.s test.bin

tests: emulate assemble
	cd ../armv8_testsuite && ./run -pf

led_blink: assemble2
	./assemble ../programs/led_blink.s ../programs/kernel8.img

format:
	clang-format -i assembler/*.c assembler/*.h emulator/*.c emulator/*.h
