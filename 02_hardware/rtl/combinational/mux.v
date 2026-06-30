module mux2(
  input   a, 
  input   b, 
  input   sel, 
  output  out
);

  assign out = (a & ~sel) | (b & sel);

endmodule

module mux64(
  input  [63:0] a,
  input  [63:0] b,
  input         sel,
  output [63:0] out
);

  assign out = sel ? a : b;

endmodule 
