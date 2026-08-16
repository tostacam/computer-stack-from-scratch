`include "address_space.svh"

module soc(
  input  logic       clk,
  input  logic       reset,
  input  logic       uart_rx,
  output logic       uart_rx,
  input  logic [3:0] sw,
  output logic [3:0] led
);

// CPU <-> Instruction ROM
logic [31:0] instruction_address;
logic [31:0] instruction_data;

// CPU <-> Data Bus
logic [31:0] data_address;
logic [31:0] data_write;
logic [31:0] data_read;
logic        mem_read;
logic        mem_write;

// Device selects
logic rom_select;
logic ram_select;
logic gpio_select;
logic uart_select;

// Device read data
logic [31:0] ram_read_data;
logic [31:0] gpio_read_data;
logic [31:0] uart_read_data;

// CPU
cpu u_cpu(
  .clk(clk),
  .reset(reset),
  .instruction_address(instruction_address),
  .instruction_data(instruction_data),
  .data_address(data_address),
  .data_write(data_write),
  .data_read(data_read),
  .mem_read(mem_read),
  .mem_write(mem_write)
);

// Instruction ROM
rom u_rom(
  .address(instruction_address),
  .data(instruction_data)  
);

// Data RAM
ram u_ram(
  .clk(clk),
  .address(data_address),
  .write_data(data_write),
  .read_data(ram_read_data),
  .write_enable(mem_write && ram_select)
);

// GPIO
gpio u_gpio(
  .clk(clk),
  .address(data_address),
  .write_data(data_write),
  .read_data(gpio_read_data),
  .write_enable(mem_write && gpio_select),
  .switches(sw),
  .leds(led)
);

// UART
uart u_uart(
  .clk(clk),
  .address(data_address),
  .write_data(data_write),
  .read_data(uart_read_data),
  .write_enable(mem_write && uart_select),
  .rx(uart_rx),
  .tx(uart_tx)
);

// address decoder
always_comb begin
  rom_select  = 0;
  ram_select  = 0;
  gpio_select = 0;
  uart_select = 0;

  if (address < 32'h00010000)
    rom_select = 1;
  else if (address < 32'h00020000)
    ram_select = 1;
  else if (address == 32'h20000000)
    gpio_select = 1;
  else if (address == 32'h20000010)
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
      data_read = 32'h00000000;
  endcase 
end 

endmodule 
