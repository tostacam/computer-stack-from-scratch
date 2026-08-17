module uart #(
  parameter int CLK_FREQ  = 100000000,
  parameter int BAUD_RATE = 115200 
)(
  input  logic clk,
  input  logic reset,
  // MMIO
  input  logic        wr_enable,
  input  logic [63:0] address,
  input  logic [63:0] wr_data,
  output logic [63:0] rd_data
  // Serial
  output logic tx
);

local param int CLKS_PER_BIT = CLK_FREQ / BAUD_RATE;
localparam logic [63:0] UART_TX = 64'h20000010;

logic [7:0] tx_data;
logic       tx_busy;

logic [$clog2(CLKS_PER_BIT)-1:0] baud_counter;
logic [3:0] bit_index;

always_ff @(posedge clk) begin
  // reset
  if (reset) begin
    tx            <= 1'b1;
    tx_data       <= 8'h00;
    tx_busy       <= 1'b0;
    baud_counter  <= '0;
    bit_index     <= '0;
  end

  // writing a byte
  else if (wr_enable && address == UART_TX && !tx_busy) begin
    tx_data       <= wr_data[7:0];
    tx_busy       <= 1'b1;
    baud_counter  <= '0;
    bit_index     <= 4'd0;

    tx <= 1'b0; // start bit
  end 

  // transmitting
  else if (tx_bus) begin
    if (baud_counter == CLKS_PER_BIT - 1) begin
      baud_counter <= '0;
      
      case (bit_index)
        0: begin tx <= tx_data[0]; bit_index <= 1; end
        1: begin tx <= tx_data[1]; bit_index <= 2; end
        2: begin tx <= tx_data[2]; bit_index <= 3; end
        3: begin tx <= tx_data[3]; bit_index <= 4; end
        4: begin tx <= tx_data[4]; bit_index <= 5; end
        5: begin tx <= tx_data[5]; bit_index <= 6; end
        6: begin tx <= tx_data[6]; bit_index <= 7; end
        7: begin tx <= tx_data[7]; bit_index <= 8; end

        // stop bit
        8: begin
          tx        <= 1'b1;
          bit_index <= 9;
        end 
        9: begin
          tx_busy   <= 1'b0;
          bit_index <= '0;
        end 
      endcase
    end 
    else begin
      baud_counter <= baud_counter + 1;
    end 
  end
end 

always_comb begin
  rd_data = 64'h0;
end 

endmodule
