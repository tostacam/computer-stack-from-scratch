SRC = primitives/*.v

test_nand: 
	xvlog $(SRC) tests/tb_nand.v
	xelab tb_nand -debug typical
	xsim tb_nand -runall

test_gates:
	xvlog $(SRC) tests/tb_gates.v
	xelab tb_gates -debug typical
	xsim tb_gates -runall

clean: 
	rm -rf xsim.dir *.log *.jou *.pb *.wdb *.vcd
