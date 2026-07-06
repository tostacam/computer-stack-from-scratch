module immediate_generator #(
  parameter WIDTH = 64
)(
  input  logic [31:0] instruction,
  output logic [WIDTH-1:0] immediate  
);

always_comb begin
  case (instruction[6:0])
    // I-type (LOAD, OP-IMM, JALR)
    7'b0000011,
    7'b0010011,
    7'b1100111:
      immediate = {{52{instruction[31]}}, instruction[31:20]};
    // S-type (STORE)
    7'b0100011:
      immediate = {{52{instruction[31]}},
                      instruction[31:25],
                      instruction[11:7]};
    // B-type (BRANCH)
    7'b1100011:
      immediate = {{51{instruction[31]}},
                      instruction[31],
                      instruction[7],
                      instruction[30:25],
                      instruction[11:8],
                      1'b0};
    // U-type (LUI, AUIPC)
    7'b0110111,
    7'b0010111:
      immediate = {{32{instruction[31]}},
                      instruction[31:12],
                      12'b0};
    // J-type (JAL)
    7'b1101111:
      immediate = {{43{instruction[31]}},
                      instruction[31],
                      instruction[19:12],
                      instruction[20],
                      instruction[30:21],
                      1'b0};
    default:
      immediate = '0;
  endcase
end

endmodule
