module register_tb;

  logic clk, wr_enable;
  logic [63:0] d, q;

  register u_register(
    .clk(clk),
    .wr_enable(wr_enable),
    .d(d),
    .q(q)
  );

  // clock setup
  initial clk = 0;
  always #1 clk = ~clk;

  initial begin
    // wr_enable = 1, 1st write
    wr_enable = 1;
    d = 64'h0123456789ABCDEF;
    @(posedge clk); #1;
    assert(q == 64'h0123456789ABCDEF)
      else $fatal();

    // wr_enable = 0, no write
    wr_enable = 0;
    d = 64'hDEADBEEFCAFEBABE;
    @(posedge clk); #1;
    assert(q == 64'h0123456789ABCDEF)
      else $fatal();

    // wr_enable = 1, 2nd write
    wr_enable = 1;
    d = 64'hFFFFFFFFFFFFFFFF;
    @(posedge clk); #1;
    assert(q == 64'hFFFFFFFFFFFFFFFF)
      else $fatal();

    // wr_enable = 0, no write
    wr_enable = 0;
    d = 64'h0123456789ABCDEF;
    @(posedge clk); #1;
    assert(q == 64'hFFFFFFFFFFFFFFFF)
      else $fatal();
     
    $display("ALL TESTS PASSED.");
    $finish;
  end 

endmodule
