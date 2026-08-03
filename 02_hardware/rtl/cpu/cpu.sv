module cpu(
  input logic clk,
  input logic reset,

  output logic [63:0] debug_pc,
  output logic [31:0] debug_instruction,
  output logic [63:0] debug_rf [31:0],
  output logic  [7:0] debug_ram [4096-1:0]
);

// fetch: program counter
logic        clear;
logic        jump_enable;
logic [63:0] jump_address;
logic [63:0] pc;
/// fetch: instruction memory
logic [31:0] instruction;

// decode: register file
logic [6:0]   opcode;
logic [4:0]   rd;
logic [2:0]   funct3;
logic [4:0]   rs1;
logic [4:0]   rs2;
logic [6:0]   funct7;
logic [63:0]  rs1_data;
logic [63:0]  rs2_data;

// execute: alu
logic [63:0] alu_b_input;
logic        alu_zero;
logic [63:0] alu_result;

// memory access: ram
logic [63:0] ram_data;

// write back: mux
logic [63:0] rf_wr_data;

// control: control unit
logic       alu_src;
logic       mem_to_reg;
logic       reg_write;
logic       mem_read;
logic       mem_write;
logic       branch;
logic [2:0] alu_op;
// control: immediate control unit
logic [63:0] immediate;
// control: alu control
logic [3:0] alu_control;

// fetch
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

// decode
instruction_decoder u_inst_dec(
  .instruction(instruction),
  .opcode(opcode),
  .rd(rd),
  .funct3(funct3),
  .rs1(rs1),
  .rs2(rs2),
  .funct7(funct7)
);

register_file u_rf(
  .clk(clk),
  .wr_data(rf_wr_data),
  .wr_enable(reg_write),
  .rs1(rs1),
  .rs2(rs2),
  .rd(rd),
  .rd1(rs1_data),
  .rd2(rs2_data)
);

immediate_generator u_imm_gen(
  .instruction(instruction),
  .immediate(immediate)
);

control_unit u_ctrl_unit(
  .opcode(opcode),
  .alu_src(alu_src),
  .mem_to_reg(mem_to_reg),
  .reg_write(reg_write),
  .mem_read(mem_read),
  .mem_write(mem_write),
  .branch(branch),
  .alu_op(alu_op)
);

// execute
alu_control u_alu_ctrl(
  .alu_op(alu_op),
  .funct3(funct3),
  .funct7(funct7),
  .alu_control(alu_control)
);

assign alu_b_input = alu_src ? immediate : rs2_data;

alu u_alu(
  .a(rs1_data),
  .b(alu_b_input),
  .alu_control(alu_control),
  .out(alu_result),
  .f_zero(alu_zero)
);

// memory access
ram u_ram(
  .clk(clk),
  .wr_enable(mem_write),
  .address(alu_result),
  .wr_data(rs2_data),
  .rd_data(ram_data)
);

// write back 
assign rf_wr_data = mem_to_reg ? ram_data : alu_result; 

// update pc
branch_control u_branch_ctrl(
  .branch(branch),
  .funct3(funct3),
  .alu_zero(alu_zero),
  .alu_result(alu_result),
  .jump_enable(jump_enable)  
);

// debug signals
assign debug_pc = pc;
assign debug_instruction = instruction;
assign debug_rf = u_rf.registers;
assign debug_ram = u_ram.memory;

endmodule
