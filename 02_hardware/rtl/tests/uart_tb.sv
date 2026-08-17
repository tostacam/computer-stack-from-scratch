module uart_tb;

  logic clk;
  logic reset;

  logic        wr_enable;
  logic [63:0] address;
  logic [63:0] wt_data;
  logic [63:0] rd_data;

  logic tx;

  uart #(
    .CLK_FREQ(10),
    .BAUD_RATE(1)  
  ) u_uart (
    .clk(clk),
    .reset(reset),
    .wr_enable(wr_enable),
    .address(address),
    .wr_data(wr_data),
    .rd_data(rd_data),
    .tx(tx)
  );

  // 10 time units
  initial clk = 0;
  always #5 clk = ~clk;

  initial begin
    // reset
    reset = 1;
    wr_enable = 0;
    address = 0;
    wr_data = 0;

    repeat (2) @(posedge clk);

    reset = 0;
    
    // transmit 'A' (0x41)
    @(posedge clk);
    wr_enable = 1;
    address = 64'h200000010;
    wr_data = 64'h41;

    @(posedge clk);
    wr_enable = 0;

    // wait for transmission
    repeat (12)
      @(posedge clk);

    $finish;
  end 

endmodule
