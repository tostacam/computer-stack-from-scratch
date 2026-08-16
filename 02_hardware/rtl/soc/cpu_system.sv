module cpu_system(
  input  logic clk,
  input  logic reset,
  output logic state,
  output logic [1:0] trap 
);

u_cpu(
  .clk(),
  .reset(),
  .state(),
  .trap()  
);

u_rom(
  .address(),
  .instruction()
);

u_ram(
  .clk(),
  .funct3(),
  .wr_enable(),
  .wr_data(),
  .rd_data()  
);

endmodule
