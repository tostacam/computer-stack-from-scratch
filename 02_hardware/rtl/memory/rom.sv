module rom #(
  parameter MEM_SIZE = 1024,
  parameter INST_SIZE = 32
)(
  input  logic [63:0] address,
  output logic [INST_SIZE-1:0] instruction
);

logic [7:0] memory [0:MEM_SIZE-1];
localparam ADDR_WIDTH = $clog2(MEM_SIZE);
logic [ADDR_WIDTH-1:0] mem_addr;
assign mem_addr = address[ADDR_WIDTH-1:0];

assign instruction = {
  memory[mem_addr + 3],  
  memory[mem_addr + 2],  
  memory[mem_addr + 1],  
  memory[mem_addr]
};

endmodule
