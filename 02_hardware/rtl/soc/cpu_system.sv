// CPU + ROM + RAM (for regression testing)

module cpu_system(
  input  logic clk,
  input  logic reset,
  output logic state,
  output logic [1:0] trap,

  output logic [63:0] debug_pc,
  output logic [31:0] debug_instruction,
  output logic [63:0] debug_rf [31:0],
  output logic  [7:0] debug_ram [4096-1:0],
  output logic [63:0] debug_imm,
  output logic [63:0] debug_jmp_addr
);

// instruction bus
logic [63:0] instruction_address;
logic [31:0] instruction_data;

// data bus
logic [63:0] data_address;
logic [63:0] data_write;
logic [63:0] data_read;
logic        mem_read;
logic        mem_write;

// CPU
cpu u_cpu(
  .clk(clk),
  .reset(reset),
  .state(state),
  .trap(trap),
  // instruction bus
  .instruction_address(instruction_address),
  .instruction_data(instruction_data),
  // data bus
  .data_address(data_address),
  .data_write(data_write),
  .data_read(data_read),
  .dt_mem_read(mem_read),
  .dt_mem_write(mem_write)
);

// ROM
rom u_rom(
  .address(instruction_address),
  .instruction(instruction_data)
);

// RAM
ram u_ram(
  .clk(clk),
  .word_size(u_cpu.word_size),
  .wr_enable(mem_write),
  .rd_enable(mem_read),
  .address(data_address),
  .wr_data(data_write),
  .rd_data(data_read)
);

//debug signals
assign debug_pc = u_cpu.pc;
assign debug_instruction = u_cpu.instruction;
assign debug_rf = u_cpu.u_rf.registers;
assign debug_ram = u_ram.memory;
assign debug_imm = u_cpu.u_imm_gen.immediate;
assign debug_jmp_addr = u_cpu.u_pc.jump_address;

endmodule
