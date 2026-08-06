`include "alu_ops.svh"

module alu_control(
  input  logic [2:0] alu_op,
  input  logic [2:0] funct3,
  input  logic [6:0] funct7,
  output logic [3:0] alu_control  
);

always_comb begin
  alu_control = `ALU_OP_ADD;
  case (alu_op)
    
    // 000 -> ADD
    3'b000:
      alu_control = `ALU_OP_ADD;  // ADD
    
    // 001 -> R-TYPE decode
    3'b001: begin
      case (funct3)
        3'b000: alu_control = (funct7 == 7'b0100000) ? `ALU_OP_SUB : `ALU_OP_ADD; // ADD, SUB
        3'b001: alu_control = `ALU_OP_SLL;                                        // SLL
        3'b010: alu_control = `ALU_OP_SLT;                                        // SLT
        3'b011: alu_control = `ALU_OP_SLTU;                                       // SLTU
        3'b100: alu_control = `ALU_OP_XOR;                                        // XOR
        3'b101: alu_control = (funct7 == 7'b0100000) ? `ALU_OP_SRA : `ALU_OP_SRL; // SRL, SRA
        3'b110: alu_control = `ALU_OP_OR;                                         // OR
        3'b111: alu_control = `ALU_OP_AND;                                        // AND
      endcase
    end

    // 010 -> BRANCH compare
    3'b010: begin
      case (funct3)
        3'b000,                             // BEQ
        3'b001: alu_control = `ALU_OP_SUB;  // BNE
        3'b100,                             // BLT
        3'b101: alu_control = `ALU_OP_SLT;  // BGE
        3'b110,                             // BLTU
        3'b111: alu_control = `ALU_OP_SLTU; // BGEU
        default: alu_control = '0;
      endcase
    end 

    // 011 -> I-type decode
    3'b011: begin
      case (funct3)
        3'b000: alu_control = `ALU_OP_ADD;                                        // ADDI
        3'b001: alu_control = `ALU_OP_SLL;                                        // SLLI
        3'b010: alu_control = `ALU_OP_SLT;                                        // SLTI
        3'b011: alu_control = `ALU_OP_SLTU;                                       // SLTUI
        3'b100: alu_control = `ALU_OP_XOR;                                        // XORI
        3'b101: alu_control = (funct7 == 7'b0100000) ? `ALU_OP_SRA : `ALU_OP_SRL; // SRLI, SRAI
        3'b110: alu_control = `ALU_OP_OR;                                         // ORI
        3'b111: alu_control = `ALU_OP_AND;                                        // ANDI
      endcase
    end 
    
    // 100 -> LUI
    3'b100:
      alu_control = `ALU_OP_PASS_B;

    // 101 -> AUIPC
    3'b101:
      alu_control = `ALU_OP_ADD;

    // 110 -> JAL/JALR
    3'b110: 
      alu_control = `ALU_OP_ADD;

    // 111 -> SYSTEM
    3'b111:
      alu_control = `ALU_OP_PASS_B;

    default:
      alu_control = `ALU_OP_ADD;

  endcase 
end

endmodule
