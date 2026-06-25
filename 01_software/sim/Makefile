CC = clang
CFLAGS = -Wall -Wextra -Ilogic -Icircuits

LOGIC_SRCS = logic/*.c
CIRCUITS_SRCS = circuits/*.c

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

clean:
	rm -f tests/test_nand tests/test_gates tests/test_bus tests/test_adders tests/test_mux \
		tests/test_bitwise tests/test_shifters
