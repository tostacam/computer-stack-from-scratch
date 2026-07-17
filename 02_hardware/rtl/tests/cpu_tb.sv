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

    $display("-------------------------");
    $display(" PC    INST        x1  x2  x3   ALU");
    $display("-------------------------");

    $display("%4d  %08h   %2d  %2d  %2d   %2d",
      u_cpu.pc,
      u_cpu.instruction,
      u_cpu.u_rf.registers[1],
      u_cpu.u_rf.registers[2],
      u_cpu.u_rf.registers[3],
      u_cpu.alu_result
    );

    repeat (6) begin
      @(posedge clk); #1;

      $display("%4d  %08h   %2d  %2d  %2d   %2d",
        u_cpu.pc,
        u_cpu.instruction,
        u_cpu.u_rf.registers[1],
        u_cpu.u_rf.registers[2],
        u_cpu.u_rf.registers[3],
        u_cpu.alu_result
      );
    end 

    $display("-------------------------");
    $display("RAM[0] = %0d", u_cpu.u_ram.memory[0]);

    $finish;
  end 

endmodule
