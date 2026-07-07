module program_counter_tb;

  logic        clk;
  logic        clear;
  logic        jump_enable;
  logic [63:0] jump_address;
  logic [63:0] pc;

  program_counter u_pc(
    .clk(clk),
    .clear(clear),
    .jump_enable(jump_enable),
    .jump_address(jump_address),
    .pc(pc)
  );

  // clock setup
  initial clk = 0;
  always #1 clk = ~clk;

  initial begin
    // clear, pc = 0
    clear = 1;

    @(posedge clk); #1
    assert(pc == 0)
      else $fatal();

    // pc = 4
    clear = 0;

    @(posedge clk); #1;
    assert(pc == 4)
      else $fatal();
  end 

endmodule;
