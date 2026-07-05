module program_counter #(
  parameter WIDTH = 64
)(
  input  logic             clk,
  input  logic             clear,
  input  logic             jump_enable,
  input  logic [WIDTH-1:0] jump_address,
  output logic [WIDTH-1:0] pc
);

always_ff @(posedge clk)
  if (clear)
    pc <= '0;
  else if (jump_enable)
    pc <= jump_address;
  else 
    pc <= pc + 4;

endmodule
