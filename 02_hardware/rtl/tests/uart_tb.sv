module uart_tb;

  logic clk;
  logic reset;

  logic        wr_enable;
  logic [63:0] address;
  logic [63:0] wr_data;
  logic [63:0] rd_data;

  logic tx;
  localparam int CLKS_PER_BIT = 10;

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

  task check_bit (input logic expected);
    repeat (CLKS_PER_BIT)
      @(posedge clk);

    assert(tx === expected)
      else $error(
        "UART bit incorrect: expected %b, got %b",
        expected,
        tx  
      );
  endtask

  initial begin
    // reset
    reset = 1;
    wr_enable = 0;
    address = 0;
    wr_data = 0;

    repeat (2) @(posedge clk);

    assert(tx === 1'b1)
      else $error("TX should be idle high during reset");

    reset = 0;
    
    // transmit 'A' (0x41)
    @(posedge clk);
    wr_enable = 1;
    address = 64'h200000010;
    wr_data = 64'h41;

    @(posedge clk);
    wr_enable = 0;

    // checking uart 
    check_bit(1'b0);  // start
    check_bit(1'b1);  // bit 0
    check_bit(1'b0);  // bit 1
    check_bit(1'b0);  // bit 2
    check_bit(1'b0);  // bit 3
    check_bit(1'b0);  // bit 4
    check_bit(1'b0);  // bit 5
    check_bit(1'b1);  // bit 6
    check_bit(1'b0);  // bit 7
    check_bit(1'b1);  // stop

    // transmission complete
    repeat (2) @(posedge clk);

    assert(u_uart.tx_busy === 1'b0)
      else $error("UART should no longer be busy");

    assert(tx === 1'b1)
      else $error("TX should return to idle high");

    $display("TESTS passed");
    $finish;
  end 

endmodule
