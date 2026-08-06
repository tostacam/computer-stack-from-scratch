`include "control_opcodes.svh"

module control_unit(
  input  logic [6:0] opcode,
  output logic alu_src_a,
  output logic alu_src_b,
  output logic mem_to_reg,
  output logic reg_write,
  output logic mem_read,
  output logic mem_write,
  output logic branch,
  output logic [2:0] alu_op 
);

always_comb begin
  // default values
  alu_src_a  = 0;
  alu_src_b  = 0;
  mem_to_reg = 0;
  reg_write  = 0;
  mem_read   = 0;
  mem_write  = 0;
  branch     = 0;
  alu_op     = '0;

  case (opcode)
    // R-Type (ADD, SUB, AND, OR, ...)
    `OPCODE_RTYPE: begin
      reg_write = 1;
      alu_op    = 3'b001;
    end

    // I-Type Arithmetic (ADDI, ANDI, ...)
    `OPCODE_ITYPE: begin
      alu_src_b = 1;
      reg_write = 1;
      alu_op    = 3'b011;
    end

    // LOAD
    `OPCODE_LOAD: begin
      alu_src_b  = 1;
      mem_to_reg = 1;
      reg_write  = 1;
      mem_read   = 1;
      alu_op     = 3'b000;
    end

    // STORE
    `OPCODE_STORE: begin
      mem_write = 1;
      alu_src_b = 1;
      alu_op    = 3'b000;
    end

    // BRANCH
    `OPCODE_BRANCH: begin
      branch  = 1;
      alu_op  = 3'b010;
    end

   // LUI
    `OPCODE_LUI: begin
      alu_src_b = 1;
      reg_write = 1;
      alu_op    = 3'b100;
    end

    // AUIPC
    `OPCODE_AUIPC: begin
      alu_src_a = 1;
      alu_src_b = 1;
      reg_write = 1;
      alu_op    = 3'b101;
    end

    // JAL
    `OPCODE_JAL: begin
      reg_write = 1;
      alu_op    = 3'b110;
    end

    // JALR
    `OPCODE_JALR: begin
      alu_src_b = 1;
      reg_write = 1;
      alu_op    = 3'b110;
    end

    // SYSTEM
    `OPCODE_SYSTEM: begin
      alu_op    = 3'b111;
    end 

    default: begin
    end
  endcase 
end

endmodule
