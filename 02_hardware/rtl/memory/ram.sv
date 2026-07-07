module ram #(
  parameter MEM_SIZE = 4096  
)(
  input  logic        clk,
  input  logic        wr_enable,
  input  logic [63:0] address, 
  input  logic [63:0] wr_data,
  output logic [63:0] rd_data
);

// raw memory
logic [7:0] memory [0:MEM_SIZE-1];

// memory address
localparam ADDR_WIDTH = $clog2(MEM_SIZE);
logic [ADDR_WIDTH-1:0] mem_addr;
assign mem_addr = address[ADDR_WIDTH-1:0];

// combinational read
assign rd_data = {
  memory[mem_addr + 7],
  memory[mem_addr + 6],
  memory[mem_addr + 5],
  memory[mem_addr + 4],
  memory[mem_addr + 3],
  memory[mem_addr + 2],
  memory[mem_addr + 1],
  memory[mem_addr]
};

// sequential write
always_ff @(posedge clk) begin
  if (wr_enable) begin
    memory[mem_addr] <= wr_data[7:0];
    memory[mem_addr + 1] <= wr_data[15:8];
    memory[mem_addr + 2] <= wr_data[23:16];
    memory[mem_addr + 3] <= wr_data[31:24];
    memory[mem_addr + 4] <= wr_data[39:32];
    memory[mem_addr + 5] <= wr_data[47:40];
    memory[mem_addr + 6] <= wr_data[55:48];
    memory[mem_addr + 7] <= wr_data[63:56];
  end 
end 
endmodule
