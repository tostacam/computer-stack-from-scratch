module dff_tb;

  logic clk, d, q;

  dff u_dff(
    .clk(clk),
    .d(d),
    .q(q)
  );

  // clock setup
  initial clk = 0;
  always #1 clk = ~clk;

  initial begin
    // d = 0 + clk -> q = 0
    d = 0;
    @(posedge clk); #1;
    assert(q == 0)
      else $fatal();

    // d = 1 + clk -> q = 1
    d = 1; 
    @(posedge clk); #1;
    assert(q == 1)
      else $fatal();
    
    // d = 0 + clk -> q = 0
    d = 0;
    @(posedge clk); #1;
    assert(q == 1)
      else $fatal();

    $display("TESTS PASSED.");
    $finish;
  end 

endmodule
