PRIM_SRC = primitives/*.sv
COMB_SRC = combinational/*.sv
VFLAGS = --lint-only -Wno-MULTITOP

test_dff:
	xvlog --sv $(PRIM_SRC) tests/dff_tb.sv
	xelab dff_tb -debug typical
	xsim dff_tb -runall

test_register:
	xvlog --sv $(PRIM_SRC) tests/register_tb.sv
	xelab register_tb -debug typical
	xsim register_tb -runall

clean: 
	rm -rf xsim.dir *.log *.jou *.pb *.wdb *.vcd
