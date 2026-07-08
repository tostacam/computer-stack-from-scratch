PRIM_SRC = primitives/*.sv
MEM_SRC = memory/*.sv
CPU_SRC = cpu/*.sv
VFLAGS = --lint-only -Wno-MULTITOP

test_dff:
	xvlog --sv $(PRIM_SRC) tests/dff_tb.sv
	xelab dff_tb -debug typical
	xsim dff_tb -runall

test_register:
	xvlog --sv $(PRIM_SRC) tests/register_tb.sv
	xelab register_tb -debug typical
	xsim register_tb -runall

test_rom:
	xvlog --sv $(MEM_SRC) tests/rom_tb.sv
	xelab rom_tb -debug typical
	xsim rom_tb -runall

test_ram:
	xvlog --sv $(MEM_SRC) tests/ram_tb.sv
	xelab ram_tb -debug typical
	xsim ram_tb -runall

test_program_counter:
	xvlog --sv $(CPU_SRC) tests/program_counter_tb.sv
	xelab program_counter_tb -debug typical
	xsim program_counter_tb -runall

test_cpu:
	xvlog --sv $(CPU_SRC) $(MEM_SRC) tests/cpu_tb.sv
	xelab cpu_tb -debug typical
	xim cpu_tb -runall 

cpu_compile:
	verilator $(VFLAGS) cpu/cpu.sv $(MEM_SRC)

clean: 
	rm -rf xsim.dir *.log *.jou *.pb *.wdb *.vcd
