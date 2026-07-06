module control_unit(
  input  logic [6:0] opcode,
  output logic alu_src,
  output logic mem_to_reg,
  output logic reg_write,
  output logic mem_read,
  output logic mem_write,
  output logic branch,
  output logic [1:0] alu_op 
);

always_comb begin
  // default values
  alu_src    = 0;
  mem_to_reg = 0;
  reg_write  = 0;
  mem_read   = 0;
  mem_write  = 0;
  branch     = 0;
  alu_op     = '0;

  case (opcode)
    // R-Type (ADD, SUB, AND, OR, ...)
    7'b0110011: begin
      alu_src    = 0;
      reg_write  = 1;
    end

    // I-Type Arithmetic (ADDI, ANDI, ...)
    7'b0010011: begin
      alu_src    = 1;
      reg_write  = 1;
    end

    // LOAD
    7'b0000011: begin
      alu_src    = 1;
      mem_to_reg = 1;
      reg_write  = 1;
      mem_read   = 1;
    end

    // STORE
    7'b0100011: begin
      mem_write = 1;
      alu_src   = 1;
    end

    // BRANCH
    7'b1100011: begin
      branch = 1;
    end

   // LUI
    7'b0110111: begin
      alu_src = 1;
      reg_write = 1;
    end

    // AUIPC
    7'b0010111: begin
      alu_src = 1;
      reg_write = 1;
    end
    default: begin
    end
  endcase 
end

endmodule
