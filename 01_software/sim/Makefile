CC = clang
CFLAGS = -Wall -Wextra -Ilogic -Icircuits -Icpu -Itests

LOGIC_SRCS = logic/*.c
CIRCUITS_SRCS = circuits/*.c
CPU_SRCS = cpu/*.c
TEST_UTIL = tests/utilities.c

test_nand:
	$(CC) $(CFLAGS) tests/test_nand.c $(LOGIC_SRCS) -o tests/test_nand

test_gates:
	$(CC) $(CFLAGS) tests/test_gates.c $(LOGIC_SRCS) -o tests/test_gates

test_bus:
	$(CC) $(CFLAGS) tests/test_bus.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) -o tests/test_bus

test_adders:
	$(CC) $(CFLAGS) tests/test_adders.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) -o tests/test_adders
 
test_mux:
	$(CC) $(CFLAGS) tests/test_mux.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) -o tests/test_mux

test_bitwise:
	$(CC) $(CFLAGS) tests/test_bitwise.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) -o tests/test_bitwise

test_shifters:
	$(CC) $(CFLAGS) tests/test_shifters.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) -o tests/test_shifters

test_comparators:
	$(CC) $(CFLAGS) tests/test_comparators.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) -o tests/test_comparators

test_dec_enc:
	$(CC) $(CFLAGS) tests/test_dec_enc.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) $(CPU_SRCS) $(TEST_UTIL) -o tests/test_dec_enc

test_alu:
	$(CC) $(CFLAGS) tests/test_alu.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) $(CPU_SRCS) $(TEST_UTIL) -o tests/test_alu

test_flip_flop:
	$(CC) $(CFLAGS) tests/test_flip_flop.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) $(CPU_SRCS)  -o tests/test_flip_flop

test_register:
	$(CC) $(CFLAGS) tests/test_register.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) $(CPU_SRCS)  -o tests/test_register

test_counter:
	$(CC) $(CFLAGS) tests/test_counter.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) $(CPU_SRCS) $(TEST_UTIL) -o tests/test_counter

test_program_counter:
	$(CC) $(CFLAGS) tests/test_program_counter.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) $(CPU_SRCS) $(TEST_UTIL) -o tests/test_program_counter

test_register_file:
	$(CC) $(CFLAGS) tests/test_register_file.c $(LOGIC_SRCS) $(CIRCUITS_SRCS) $(CPU_SRCS) $(TEST_UTIL) -o tests/test_register_file

clean:
	rm -f tests/test_nand tests/test_gates tests/test_bus tests/test_adders tests/test_mux \
		tests/test_bitwise tests/test_shifters tests/test_comparators tests/test_dec_enc \
		tests/test_alu tests/test_flip_flop tests/test_register tests/test_counter tests/test_program_counter \
		tests/test_register_file
