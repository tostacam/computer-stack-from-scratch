sim: 
	$(MAKE) -C 01_software/sim cpu_compile

rtl: 
	$(MAKE) -C 02_hardware/rtl cpu_compile
	$(MAKE) -C 02_hardware/rtl soc_compile

all: sim rtl

test:
	$(MAKE) -C 03_validation run_all_tests

solo_test: sim rtl
	$(MAKE) -C 03_validation clean
	$(MAKE) -C 03_validation run_solo_test

clean:
	$(MAKE) -C 01_software/sim clean
	$(MAKE) -C 02_hardware/rtl clean
	$(MAKE) -C 03_validation clean

ci: clean all test
