`include "control_opcodes.svh"
`include "writeback_opcodes.svh"
`include "pc_opcodes.svh"

module control_unit(
  input  logic [6:0] opcode,
  output logic alu_src_a,
  output logic alu_src_b,
  output logic [1:0] wb_src,
  output logic reg_write,
  output logic mem_read,
  output logic mem_write,
  output logic [1:0] pc_src,
  output logic [2:0] alu_op 
);

always_comb begin
  // default values
  alu_src_a  = 0;
  alu_src_b  = 0;
  wb_src     = `WB_ALU;
  reg_write  = 0;
  mem_read   = 0;
  mem_write  = 0;
  pc_src     = `PC_NEXT;
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
      wb_src     = `WB_MEM;
      alu_src_b  = 1;
      reg_write  = 1;
      mem_read   = 1;
      alu_op     = 3'b000;
    end

    // STORE
    `OPCODE_STORE: begin
      wb_src    = `WB_MEM;
      mem_write = 1;
      alu_src_b = 1;
      alu_op    = 3'b000;
    end

    // BRANCH
    `OPCODE_BRANCH: begin
      pc_src  = `PC_BRANCH;
      alu_op  = 3'b010;
    end

   // LUI
    `OPCODE_LUI: begin
      alu_src_a = 1;
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
      wb_src    = `WB_PC4;
      reg_write = 1;
      pc_src    = `PC_JAL;
      alu_op    = 3'b110;
    end

    // JALR
    `OPCODE_JALR: begin
      wb_src    = `WB_PC4;
      alu_src_b = 1;
      reg_write = 1;
      pc_src    = `PC_JALR;
      alu_op    = 3'b110;
    end

    // SYSTEM
    `OPCODE_SYSTEM: begin
      alu_src_b = 1;
      alu_op    = 3'b111;
    end 

    default: begin
    end
  endcase 
end

endmodule
