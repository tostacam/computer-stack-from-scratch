module rom #(
  parameter MEM_SIZE = 1024,
  parameter INST_SIZE = 32
)(
  input  logic [63:0] address,
  output logic [INST_SIZE-1:0] instruction
);

/*
// memory
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
*/

always_comb begin
  case (address)
    64'h0000: instruction = 32'h00500093; // addi x1,x0,5
    64'h0004: instruction = 32'h00A00113; // addi x2,x0,10
    64'h0008: instruction = 32'h002081B3; // add  x3,x1,x2
    64'h000C: instruction = 32'h00302023; // sw   x3,0(x0)
    default: instruction = 32'h00000013; // nop
  endcase
end

endmodule
