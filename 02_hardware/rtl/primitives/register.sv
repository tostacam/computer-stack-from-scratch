module register #(
  parameter WIDTH = 64
)(
  input  logic clk,
  input  logic wr_enable,
  input  logic [WIDTH-1:0] d,
  output logic [WIDTH-1:0] q
);

always_ff @(posedge clk)
  if (wr_enable)
    q <= d;

endmodule
