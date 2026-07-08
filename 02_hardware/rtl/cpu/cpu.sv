module cpu(
  input logic clk,
  input logic reset  
);

// fetch
logic        clear;
logic        jump_enable;
logic [63:0] jump_address;
logic [63:0] pc;
logic [31:0] instruction;

// decode
logic [6:0] opcode;
logic [4:0] rd;
logic [2:0] funct3;
logic [4:0] rs1;
logic [4:0] rs2;
logic [6:0] funct7;

// immediate
logic [63:0] immediate;

// execute
logic [63:0] rs1_data;
logic [63:0] rs2_data;
logic        alu_zero;
logic [63:0] alu_result;
logic [63:0] ram_data;
logic [63:0] wr_data;

// control
logic alu_src;
logic mem_to_reg;
logic reg_write;
logic mem_read;
logic mem_write;
logic branch;
logic [1:0] alu_op;

// instantiate modules
program_counter pc_inst(
  .clk(clk),
  .clear(reset),
  .jump_enable(jump_enable),
  .jump_address(jump_address),
  .pc(pc)
);

rom rom_inst(
  .address(pc),
  .instruction(instruction)  
);

// fetch



// decode



// execute



// memory access



// write back





/*

instruction_decoder id_inst();

register_file rf_inst();

immediate_generator ig_inst();

alu_control alu_ctrl_inst();

alu alu_inst();

ram ram_inst();

*/

endmodule 
