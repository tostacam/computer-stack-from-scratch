TEST_FILE = tests/labels.s

test_assembler:
	python3 validate_assembler.py $(TEST_FILE)

objdump:
	riscv64-unknown-elf-objdump -d tests/assembler_file_gnu.o

clean:
	rm -f tests/*.o tests/*.hex
