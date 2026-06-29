module nand_gate(
  input   a, 
  input   b, 
  output  out
);

  // NAND is the most fundamental gate, however HDL 
  // already has a native nand() function, just 
  // writing a representation here.
  // (same with other gates: and, not, or, nor, xor)
  assign out = ~(a & b);

endmodule
