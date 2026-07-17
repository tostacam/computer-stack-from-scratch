module rom #(
  parameter MEM_SIZE = 1024,
  parameter ROM_FILE = "tests/program.hex"
)(
  input  logic [63:0] address,
  output logic [31:0] instruction
);

logic [31:0] memory [0:MEM_SIZE-1];

initial begin
  $readmemh(ROM_FILE, memory);
end 

localparam ADDR_WIDTH = $clog2(MEM_SIZE);
assign instruction = memory[address[ADDR_WIDTH+1:2]];

/*
always_comb begin
  case (address)
    64'h0000: instruction = 32'h00500093; // addi x1,x0,5
    64'h0004: instruction = 32'h00A00113; // addi x2,x0,10
    64'h0008: instruction = 32'h002081B3; // add  x3,x1,x2
    64'h000C: instruction = 32'h00302023; // sw   x3,0(x0)
    default: instruction = 32'h00000013; // nop
  endcase
end
*/

endmodule
