module alu_control(
  input  logic [1:0] alu_op,
  input  logic [2:0] funct3,
  input  logic [6:0] funct7,
  output alu_op_code alu_control  
);

always_comb begin

  case (alu_op)
    // (0,0) -> add
    2'b00:
      alu_control = ALU_OP_ADD;
    // (0,1) -> sub
    2'b01:
      alu_control = ALU_OP_SUB;
    // (1,0) -> decode funct3/funct7
    2'b10: begin
      if (funct3 == 3'b000 && funct7 == 7'b0000000)
        alu_control = ALU_OP_ADD;
      if (funct3 == 3'b000 && funct7 == 7'b0100000)
        alu_contrl = ALU_OP_SUB;
    end 
    
    // (1,1) -> LUI
    2'b11:
      alu_control = ALU_OP_PASS_B;

    default:
      alu_control = ALU_OP_ADD;
  endcase 

end

endmodule
