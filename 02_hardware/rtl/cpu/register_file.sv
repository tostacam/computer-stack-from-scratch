module register_file #(
  parameter WIDTH    = 64,
  parameter NUM_REGS = 32
)(
  input  logic             clk,
  input  logic [WIDTH-1:0] wr_data,
  input  logic             wr_enable,
  input  logic [4:0]       rs1,
  input  logic [4:0]       rs2,
  input  logic [4:0]       rd,
  output logic [WIDTH-1:0] rd1,
  output logic [WIDTH-1:0] rd2
);

logic [WIDTH-1:0] registers [0:NUM_REGS-1];

assign rd1 = (rs1 == 0) ? '0 : registers[rs1];
assign rd2 = (rs2 == 0) ? '0 : registers[rs2];

always_ff @(posedge clk)
  if (wr_enable && rd != 0)
    registers[rd] <= wr_data;

endmodule
