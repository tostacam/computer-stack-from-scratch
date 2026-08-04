module rom #(
  parameter MEM_SIZE = 1024
)(
  input  logic [63:0] address,
  output logic [31:0] instruction
);

logic [31:0] memory [0:MEM_SIZE-1];

string rom_file;

initial begin
  if (!$value$plusargs("ROM=%s", rom_file))
    rom_file = "tools/add.hex"; // default, just in case
  //$display("Loading ROM: %s", rom_file);
  $readmemh(rom_file, memory);
end 

localparam ADDR_WIDTH = $clog2(MEM_SIZE);
assign instruction = memory[address[ADDR_WIDTH+1:2]];

endmodule
