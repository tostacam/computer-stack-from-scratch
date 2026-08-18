module soc(
  input  logic clk,
  input  logic reset,
  output logic state,
  output logic [1:0] trap,
  input  logic uart_rx,
  output logic uart_tx,

  // debug signals
  output logic [63:0] debug_pc,
  output logic [31:0] debug_instruction,
  output logic [63:0] debug_rf [31:0],
  output logic  [7:0] debug_ram [4096-1:0],
  output logic [63:0] debug_imm,
  output logic [63:0] debug_jmp_addr
);

// CPU <-> Instruction Bus
logic [63:0] instruction_address;
logic [31:0] instruction_data;

// CPU <-> Data Bus
logic [63:0] data_address;
logic [63:0] data_write;
logic [63:0] data_read;
logic        mem_read;
logic        mem_write;

// Device selects
logic ram_select;
logic gpio_select;
logic uart_select;

// Device read data
logic [63:0] ram_read_data;
logic [63:0] gpio_read_data;
logic [63:0] uart_read_data;

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
  .wr_enable(mem_write && ram_select),
  .rd_enable(mem_read && ram_select),
  .address(data_address),
  .wr_data(data_write),
  .rd_data(ram_read_data)
);

// UART
uart u_uart(
  .clk(clk),
  .reset(reset),
  .wr_enable(mem_write && uart_select),
  .address(data_address),
  .wr_data(data_write),
  .rd_data(uart_read_data),
  .tx(uart_tx)
);

// address decoder
always_comb begin
  ram_select  = 0;
  gpio_select = 0;
  uart_select = 0;

  if (data_address >= 64'h0001000 && data_address < 64'h00020000)
    ram_select = 1;
  else if (data_address == 64'h20000000)
    gpio_select = 1;
  else if (data_address == 64'h20000010)
    uart_select = 1;
end 

// read_data mux
always_comb begin
  unique case (1'b1)
    ram_select: 
      data_read = ram_read_data;
    gpio_select:
      data_read = gpio_read_data;
    uart_select:
      data_read = uart_read_data;
    default:
      data_read = 64'h00000000;
  endcase 
end 

//debug signals
assign debug_pc = u_cpu.pc;
assign debug_instruction = u_cpu.instruction;
assign debug_rf = u_cpu.u_rf.registers;
assign debug_ram = u_ram.memory;
assign debug_imm = u_cpu.u_imm_gen.immediate;
assign debug_jmp_addr = u_cpu.u_pc.jump_address;

endmodule 
