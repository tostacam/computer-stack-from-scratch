module rom_tb;

  logic [63:0] address;
  logic [31:0] instruction;
  
  rom u_rom(
    .address(address),
    .instruction(instruction)  
  );

  // initialize rom
  initial begin
    // instruction #1
    u_rom.memory[0] = 8'hEF;
    u_rom.memory[1] = 8'hBE;
    u_rom.memory[2] = 8'hAD;
    u_rom.memory[3] = 8'hDE;
    // instruction #2
    u_rom.memory[4] = 8'h78;
    u_rom.memory[5] = 8'h56;
    u_rom.memory[6] = 8'h34;
    u_rom.memory[7] = 8'h12;
  end

  initial begin
    // read 1st instruction
    address = 0; #1;
    assert (instruction == 32'hDEADBEEF)
      else $fatal();
    // read 2nd instruction
    address = 4; #1;
    assert (instruction == 32'h12345678)
      else $fatal();

    $display("ALL TESTS PASSED.");
    $finish;
  end

endmodule
