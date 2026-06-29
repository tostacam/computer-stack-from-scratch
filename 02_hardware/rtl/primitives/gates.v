// 

module not_gate(
  input   a, 
  output  out
);

  assign out = ~a;

endmodule

module and_gate(
  input   a, 
  input   b, 
  output  out
);

  assign out = a & b;

endmodule

module or_gate(
  input   a, 
  input   b, 
  output  out
);

  assign out = a | b;

endmodule

module nor_gate(
  input   a, 
  input   b, 
  output  out
);

  assign out = ~(a | b);

endmodule

module xor_gate(
  input   a, 
  input   b, 
  output  out
);

  assign out = (a & ~b) | (~a & b);

endmodule
