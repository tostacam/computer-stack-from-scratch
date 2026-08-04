`include "branch_ops.svh"

module branch_control(
  input  logic        branch,
  input  logic [2:0]  funct3,
  input  logic        alu_zero,
  input  logic [63:0] alu_result,
  output logic        jump_enable 
);

always_comb begin
  jump_enable = 0;

  if (branch) begin
    case (funct3)
      `FUNCT3_BEQ:
        jump_enable = alu_zero;
      `FUNCT3_BNE:
        jump_enable = !alu_zero;
      `FUNCT3_BLT:
        jump_enable = (alu_result == 1);
      `FUNCT3_BGE:
        jump_enable = (alu_result == 0);
      `FUNCT3_BLTU:
        jump_enable = (alu_result == 1);
      `FUNCT3_BGEU:
        jump_enable = (alu_result == 0);
      default:
        jump_enable = 0;
    endcase 
  end
end

endmodule
