module cpu_tb;

  logic clk;
  logic reset;

  cpu u_cpu(
    .clk(clk),
    .reset(reset)  
  );

  // 100 MHz clock (10 ns period)
  initial begin
    clk = 0;
    forever #5 clk = ~clk;
  end 

  initial begin
    reset = 1;

    #12;
    reset = 0;

    repeat (6) begin
      @(posedge clk); #1;
      $display("PC = 0x%016h Instruction = 0x%08h",
        u_cpu.pc
        u_cpu.instruction);
    end 

    $finish;
  end 

endmodule
