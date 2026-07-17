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

endmodule
