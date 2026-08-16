module cpu(
  input  logic clk,
  input  logic reset,
  output logic state,
  output logic [1:0] trap,
  // instruction bus
  output logic [63:0] instruction_address,
  input  logic [31:0] instruction_data,
  // data bus
  output logic [63:0] data_address,
  output logic [63:0] data_write,
  input  logic [63:0] data_read,
  output logic        dt_mem_read,
  output logic        dt_mem_write
);

// fetch: program counter
logic        pc_clk;
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
logic [63:0] alu_a_input;
logic [63:0] alu_b_input;
logic        alu_zero;
logic [63:0] alu_result;

// memory access: ram
logic [63:0] ram_data;
logic  [2:0] word_size;

// write back: mux
logic [63:0] rf_wr_data;

// control: control unit
logic       alu_src_a;
logic       alu_src_b;
logic [1:0] wb_src;
logic       reg_write;
logic       mem_read;
logic       mem_write;
logic [1:0] pc_src;
logic [2:0] alu_op;
// control: immediate control unit
logic [63:0] immediate;
// control: alu control
logic [3:0] alu_control;

// fetch
program_counter u_pc(
  .clk(pc_clk),
  .clear(reset),
  .jump_enable(jump_enable),
  .jump_address(jump_address),
  .pc(pc)
);

assign instruction_address = pc;
assign instruction = instruction_data;

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
  .alu_src_a(alu_src_a),
  .alu_src_b(alu_src_b),
  .wb_src(wb_src),
  .reg_write(reg_write),
  .mem_read(mem_read),
  .mem_write(mem_write),
  .pc_src(pc_src),
  .alu_op(alu_op)
);

// execute
alu_control u_alu_ctrl(
  .alu_op(alu_op),
  .funct3(funct3),
  .funct7(funct7),
  .alu_control(alu_control)
);

assign alu_a_input = alu_src_a ? pc : rs1_data;
assign alu_b_input = alu_src_b ? immediate : rs2_data;

alu u_alu(
  .a(alu_a_input),
  .b(alu_b_input),
  .alu_control(alu_control),
  .out(alu_result),
  .f_zero(alu_zero)
);

// check cpu state
always_comb begin
  state = 0;

  if (opcode == `OPCODE_SYSTEM) begin
    state = 1;
    case (immediate)
      0: // ECALL
        trap = 0;
      1: // EBREAK
        trap = 0;
    endcase
  end

  pc_clk = !state && clk;
end

// memory access
assign data_address = alu_result;
assign data_write   = rs2_data;
assign ram_data     = data_read;
assign dt_mem_write = mem_write;
assign dt_mem_read  = mem_read;
assign word_size    = funct3;

// write back
always_comb begin
  case (wb_src) 
    `WB_ALU: begin
      rf_wr_data = alu_result;
    end 
    `WB_MEM: begin
      rf_wr_data = ram_data;
    end
    `WB_PC4: begin
      rf_wr_data = pc + 4;
    end
    default: begin
    end
  endcase 
end

// update pc
branch_control u_branch_ctrl(
  .pc_src(pc_src),
  .funct3(funct3),
  .alu_zero(alu_zero),
  .alu_result(alu_result),
  .pc(pc),
  .immediate(immediate),
  .rs1_data(rs1_data),
  .jump_enable(jump_enable),
  .jump_address(jump_address)
);

endmodule
