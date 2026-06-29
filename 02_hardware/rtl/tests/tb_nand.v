module tb_nand;

  reg   a;
  reg   b;
  wire  out;
  
  nand_gate dut(
    .a(a),
    .b(b),
    .out(out)
  );

  initial begin

    // nand(0,0) = 1
    a = 0; b = 0; #1
    if (out !== 1'b1) $fatal;
    // nand(0,1) = 1
    a = 0; b = 1; #1
    if (out !== 1'b1) $fatal;
    // nand(1,0) = 1
    a = 1; b = 0; #1
    if (out !== 1'b1) $fatal;
    // nand(1,1) = 0
    a = 1; b = 1; #1
    if (out !== 1'b0) $fatal;

  end

endmodule
