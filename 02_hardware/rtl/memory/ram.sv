module ram #(
  parameter MEM_SIZE = 4096  
)(
  input  logic        clk,
  input  logic [2:0]  funct3,
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
always_comb begin
  case (funct3)
    3'b000: begin // LB
      rd_data = {{56{memory[mem_addr][7]}}, memory[mem_addr]};
    end
    3'b001: begin // LH
      rd_data = {
        {48{memory[mem_addr + 1][7]}},
        memory[mem_addr + 1],
        memory[mem_addr]
      };
    end
    3'b010: begin // LW
      rd_data = {
        {32{memory[mem_addr + 3][7]}},
        memory[mem_addr + 3],
        memory[mem_addr + 2],
        memory[mem_addr + 1],
        memory[mem_addr]
      };
    end 
    3'b011: begin // LD
      rd_data = {
        memory[mem_addr + 7],
        memory[mem_addr + 6],
        memory[mem_addr + 5],
        memory[mem_addr + 4],
        memory[mem_addr + 3],
        memory[mem_addr + 2],
        memory[mem_addr + 1],
        memory[mem_addr]
      };  
    end
    3'b100: begin // LBU
      rd_data = {56'b0, memory[mem_addr]};
    end
    3'b101: begin // LHU
      rd_data = {
        48'b0,
        memory[mem_addr + 1],
        memory[mem_addr]  
      };
    end 
    3'b110: begin
      rd_data = {
        32'b0,
        memory[mem_addr + 3],
        memory[mem_addr + 2],
        memory[mem_addr + 1],
        memory[mem_addr]
      };
    end
    default: begin
      rd_data = '0;
    end
  endcase 
end

// sequential write
always_ff @(posedge clk) begin
  if (wr_enable) begin
    case (funct3) 
      3'b000: begin // SB
        memory[mem_addr] <= wr_data[7:0];
      end
      3'b001: begin // SH
        memory[mem_addr] <= wr_data[7:0];
        memory[mem_addr + 1] <= wr_data[15:8];
      end
      3'b010: begin // SW
        memory[mem_addr] <= wr_data[7:0];
        memory[mem_addr + 1] <= wr_data[15:8];
        memory[mem_addr + 2] <= wr_data[23:16];
        memory[mem_addr + 3] <= wr_data[31:24];
      end
      3'b011: begin // SD
        memory[mem_addr] <= wr_data[7:0];
        memory[mem_addr + 1] <= wr_data[15:8];
        memory[mem_addr + 2] <= wr_data[23:16];
        memory[mem_addr + 3] <= wr_data[31:24];
        memory[mem_addr + 4] <= wr_data[39:32];
        memory[mem_addr + 5] <= wr_data[47:40];
        memory[mem_addr + 6] <= wr_data[55:48];
        memory[mem_addr + 7] <= wr_data[63:56];
      end
      default: begin
      end 
    endcase
  end 
end 
endmodule
