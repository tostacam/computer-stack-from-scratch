PRIM_SRC = primitives/*.v
COMB_SRC = combinational/*.v
VFLAGS = --lint-only -Wno-MULTITOP

test_nand: 
	xvlog $(PRIM_SRC) tests/tb_nand.v
	xelab tb_nand -debug typical
	xsim tb_nand -runall

test_gates:
	xvlog $(PRIM_SRC) tests/tb_gates.v
	xelab tb_gates -debug typical
	xsim tb_gates -runall

test_mux:
	xvlog $(COMB_SRC) tests/tb_mux.v
	xelab tb_mux -debug typical
	xsim tb_mux -runall

test_adders:
	xvlog $(COMB_SRC) tests/tb_adders.v
	xelab tb_adders -debug typical
	xsim tb_adders -runall

test_alu:
	xvlog $(COMB_SRC) tests/tb_alu.v
	xelab tb_alu -debug typical
	xsim tb_alu -runall

clean: 
	rm -rf xsim.dir *.log *.jou *.pb *.wdb *.vcd
