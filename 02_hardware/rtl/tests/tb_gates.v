module tb_gates;

  reg a, b;
  wire and_out, not_out, or_out, nor_out, xor_out;

  and_gate dut_and(
    .a(a),
    .b(b),
    .out(and_out)
  );

  not_gate dut_not(
    .a(a),
    .out(not_out)
  );

  or_gate dut_or(
    .a(a),
    .b(b),
    .out(or_out)
  );

  nor_gate dut_nor(
    .a(a),
    .b(b),
    .out(nor_out)
  );

  xor_gate dut_xor(
    .a(a),
    .b(b),
    .out(xor_out)
  );

  initial begin
    
    // and(0,0) = 0
    a = 0; b = 0; #1;
    if (and_out !== 1'b0) $fatal;
    // and(0,1) = 0
    a = 0; b = 1; #1;
    if (and_out !== 1'b0) $fatal;
    // and(1,0) = 0
    a = 1; b = 0; #1;
    if (and_out !== 1'b0) $fatal;
    // and(1,1) = 1
    a = 1; b = 1; #1;
    if (and_out !== 1'b1) $fatal;

    // not(0) = 1
    a = 0; #1;
    if (not_out !== 1'b1) $fatal;
    // not(1) = 0
    a = 1; #1;
    if (not_out !== 1'b0) $fatal;
 
    // or(0,0) = 0
    a = 0; b = 0; #1;
    if (or_out !== 1'b0) $fatal;
    // or(0,1) = 1
    a = 0; b = 1; #1;
    if (or_out !== 1'b1) $fatal;
    // or(1,0) = 1
    a = 1; b = 0; #1;
    if (or_out !== 1'b1) $fatal;
    // or(1,1) = 1
    a = 1; b = 1; #1;
    if (or_out !== 1'b1) $fatal;
 
    // nor(0,0) = 1
    a = 0; b = 0; #1;
    if (nor_out !== 1'b1) $fatal;
    // nor(0,1) = 0
    a = 0; b = 1; #1;
    if (nor_out !== 1'b0) $fatal;
    // nor(1,0) = 0
    a = 1; b = 0; #1;
    if (nor_out !== 1'b0) $fatal;
    // nor(1,1) = 0
    a = 1; b = 1; #1;
    if (nor_out !== 1'b0) $fatal;
 
    // xor(0,0) = 0
    a = 0; b = 0; #1;
    if (xor_out !== 1'b0) $fatal;
    // xor(0,1) = 1
    a = 0; b = 1; #1;
    if (xor_out !== 1'b1) $fatal;
    // xor(1,0) = 1
    a = 1; b = 0; #1;
    if (xor_out !== 1'b1) $fatal;
    // xor(1,1) = 0
    a = 1; b = 1; #1;
    if (xor_out !== 1'b0) $fatal;

    $display("PASS: ALL TESTS COMPLETED");
  end

endmodule
