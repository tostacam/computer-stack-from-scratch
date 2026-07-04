PRIM_SRC = primitives/*.v
COMB_SRC = combinational/*.v
VFLAGS = --lint-only -Wno-MULTITOP

test_dff:
	xvlog --sv $(PRIM_SRC) tests/dff_tb.sv
	xelab dff_tb -debug typical
	xsim diff_tb -runall

test_register:
	xvlog --sv $(PRIM_SRC) tests/register_tb.sv
	xelab register_tb -debug typical
	xsim register -runall

clean: 
	rm -rf xsim.dir *.log *.jou *.pb *.wdb *.vcd
