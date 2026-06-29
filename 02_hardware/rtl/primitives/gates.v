// 

module not_gate(a, out);

  input a;
  output out;

  assign out = ~a;

endmodule

module and_gate(a, b, out);

  input a, b;
  output out;

  assign out = a & b;

endmodule

module or_gate(a, b, out);

  input a, b;
  output out;

  assign out = a | b;

endmodule

module nor_gate(a, b, out);

  input a, b;
  output out;

  assign out = ~(a | b);

endmodule

module xor_gate(a, b, out);

  input a, b;
  output out;

  assign out = (a & ~b) | (~a & b);

endmodule
