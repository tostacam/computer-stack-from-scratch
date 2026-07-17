`include "alu_ops.svh"

module alu(
  input  logic [63:0] a,
  input  logic [63:0] b,
  input  logic [3:0]  alu_control,
  output logic [63:0] out,
  output logic        f_zero
);

always_comb begin
  out = '0; 
  case (alu_control)
    `ALU_OP_ADD   : out = a + b;
    `ALU_OP_SUB   : out = a - b;
    `ALU_OP_AND   : out = a & b;
    `ALU_OP_OR    : out = a | b;
    `ALU_OP_XOR   : out = a ^ b;
    `ALU_OP_SLL   : out = a << b[5:0];
    `ALU_OP_SRL   : out = a >> b[5:0];
    `ALU_OP_SRA   : out = $signed(a) >>> b[5:0];
    `ALU_OP_SLT   : out = {63'b0, $signed(a) < $signed(b)};
    `ALU_OP_SLTU  : out = {63'b0, $unsigned(a) < $unsigned(b)};
    `ALU_OP_PASS_B: out = b;
    default       : out = '0;
  endcase
end 

assign f_zero = (out == 64'b0); // zero

endmodule
