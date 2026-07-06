module ram_tb;

  logic        clk;
  logic        wr_enable;
  logic [63:0] address;
  logic [63:0] wr_data;
  logic [63:0] rd_data;
  
  ram u_ram(
    .clk(clk),
    .wr_enable(wr_enable),
    .address(address),
    .wr_data(wr_data),
    .rd_data(rd_data)
  );

  // clock setup
  initial clk = 0;
  always #1 clk = ~clk;

  initial begin

    // ram[0] = 0xFF
    address   = 0;
    wr_data   = 64'hFF;
    wr_enable = 1;
    
    @(posedge clk); 
    
    wr_enable = 0;
    #1;
    assert(rd_data == 64'hFF)
      else $fatal();

    // ram [8] = 0xAA
    address   = 8;
    wr_data   = 64'hAA;
    wr_enable = 1;
    
    @(posedge clk); 
    
    wr_enable = 0;
    #1;
    assert(rd_data == 64'hAA)
      else $fatal();

    // ram [0] = 0xFF (unchanged)
    address   = 0;
    wr_data   = 64'hBB;
    wr_enable = 0;

    @(posedge clk); 
    
    #1;
    assert(rd_data == 64'hFF)
      else $fatal();

    // ram[0] = 0xCC (overwrite)
    address   = 0;
    wr_data   = 64'hCC;
    wr_enable = 1;

    @(posedge clk);

    #1;
    assert(rd_data == 64'hCC)
      else $fatal();
  
    $display("ALL TESTS PASSED.");
    $finish;
  end

endmodule
