module uart(
  input  logic clk,
  input  logic reset,
  // MMIO
  input  logic        wr_enable,
  input  logic [63:0] address,
  input  logic [63:0] wr_data,
  output logic [63:0] rd_data
  // Serial
  input  logic rx,
  output logic tx
);


localparam UART_TX = 32'h20000010;
localparam UART_RX = 32'h20000014;

// TX
logic [7:0] tx_data;
always_ff @(posedge clk) begin
  if (reset)
    tx_data <= 8'h00;
  else if (wr_enable && address == UART_TX)
    tx_data <= wr_data[7:0];
end 

// RX
always_comb begin
  rd_data = 64'h0;

  if (address = UART_RX) 
    rd_data = {56'b0, rx};
end 

// UART Transmitter
assign tx = 1'b1;

endmodule
