`include "soc_memory_map.svh"

module gpio #(
  parameter WIDTH = 8
) (
  input  logic clk,
  input  logic reset,
  // soc address decoder
  input  logic gpio_select,
  input  logic wr_enable,
  input  logic [3:0] address_offset,
  input  logic [31:0] wr_data,
  output logic [31:0] rd_data,
  // FPGA top level
  input  logic [WIDTH-1:0] gpio_in,
  output logic [WIDTH-1:0] gpio_out,
  output logic [WIDTH-1:0] gpio_out_enable
);

localparam logic [3:0] OUT_OFFSET = 4'h0;
localparam logic [3:0] IN_OFFSET  = 4'h4;
localparam logic [3:0] OE_OFFSET  = 4'h8;

always_ff @(posedge clk) begin
  // reset
  if (reset) begin
    gpio_out <= '0;
    gpio_out_enable <= '0;
  end
  else if (gpio_select && wr_enable) begin
    case (address_offset)
      OUT_OFFSET: gpio_out <= wr_data[WIDTH-1:0];
      OE_OFFSET:  gpio_out_enable <= wr_data[WIDTH-1:0];
    endcase 
  end 
end

always_comb begin
  case (address_offset)
    OUT_OFFSET: rd_data = gpio_out;
    IN_OFFSET:  rd_data = gpio_in;
    OE_OFFSET:  rd_data = gpio_out_enable;
    default:    rd_data = '0;
  endcase
end 

endmodule
