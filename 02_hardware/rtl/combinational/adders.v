module half_adder(
  input a,
  input b,
  output sum,
  output cout
);

  assign sum = a ^ b;
  assign cout = a & b;

endmodule

module full_adder(
  input a,
  input b,
  input cin,
  output sum,
  output cout
);

  wire h1_sum, h1_cout;
  wire h2_sum, h2_cout;

  half_adder h1(
    .a(a),
    .b(b),
    .sum(h1_sum),
    .cout(h1_cout)
  );

  half_adder h2(
    .a(cin),
    .b(h1_sum),
    .sum(h2_sum),
    .cout(h2_cout)
  );

  assign sum = h2_sum;
  assign cout = h1_cout | h2_cout;

endmodule

module add64(
  input [63:0] a,
  input [63:0] b,
  input cin,
  output [63:0] out,
  output cout
);

  wire [64:0] c;
  assign c[0] = cin;

  genvar i;
  generate
    for (i = 0; i < 64; i = i + 1) begin : FullAdder
      full_adder fa(
        .a(a[i]),
        .b(b[i]),
        .cin(c[i]),
        .sum(out[i]),
        .cout(c[i+1])
      );
    end 
  endgenerate

  assign cout = c[64];

endmodule
