CC = clang
CFLAGS = -Wall -Wextra -Ilogic

LOGIC_SRCS = logic/*.c

test_nand:
	$(CC) $(CFLAGS) tests/test_nand.c $(LOGIC_SRCS) -o tests/test_nand

clean:
	rm -f tests/test_nand
