`include "branch_ops.svh"
`include "pc_opcodes.svh"

module branch_control(
  input  logic [1:0]  pc_src,
  input  logic [2:0]  funct3,
  input  logic        alu_zero,
  input  logic [63:0] alu_result,
  input  logic [63:0] pc,
  input  logic [63:0] immediate,
  input  logic [63:0] rs1_data,
  output logic        jump_enable,
  output logic [63:0] jump_address
);

always_comb begin
  jump_enable = 0;
  jump_address = pc + immediate;

  case (pc_src)
    `PC_NEXT: begin
      jump_enable = 0;
    end 
    `PC_BRANCH: begin
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
    `PC_JAL: begin
      jump_enable = 1;
      jump_address = pc + immediate;
    end
    `PC_JALR: begin
      jump_enable = 1;
      jump_address = (rs1_data + immediate) & ~1;
    end
  endcase
end

endmodule
