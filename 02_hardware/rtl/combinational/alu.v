module alu(
  input  [63:0] a,
  input  [63:0] b,
  input  [2:0]  control,
  output [63:0] out,
  output [1:0]  flags
);

  localparam ALU_OP_ADD = 3'b000;
  localparam ALU_OP_SUB = 3'b001;
  localparam ALU_OP_AND = 3'b010;
  localparam ALU_OP_OR  = 3'b011;
  localparam ALU_OP_XOR = 3'b100;
  localparam ALU_OP_NOT = 3'b101;
  localparam ALU_OP_SHL = 3'b110;
  localparam ALU_OP_SHR = 3'b111;

  wire [63:0] add_out;
  wire        carry_out;

  add64 u_add(
    .(a),
    .b(b),
    .cin(1'b0),
    .out(add_out),
    .cout(carry_out)
  );

  always @(*) begin
    case (control)
      ALU_OP_ADD: out = add_out;
      ALU_OP_SUB: out = a - b;
      ALU_OP_NOT: out = ~a;
      ALU_OP_AND: out = a & b;
      ALU_OP_OR : out = a | b;
      ALU_OP_XOR: out = a ^ b;
      ALU_OP_SHL: out = a << b[5:0];
      ALU_OP_RHL: out = a >> b[5:0];
      default   : out = 64'b0
  end 

  assign flags[0] = (out == 64'b0); // zero
  assign flags[1] = carry_out;      // carry

endmodule
