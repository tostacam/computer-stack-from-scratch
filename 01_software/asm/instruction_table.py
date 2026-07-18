instruction_table = {
  # ---------------- R ----------------
  "add": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b000,
    "funct7": 0b0000000,
  },
  "sub": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b000,
    "funct7": 0b0100000,
  },
  "and": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b111,
    "funct7": 0b0000000,
  },
  "or": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b110,
    "funct7": 0b0000000,
  },
  "xor": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b100,
    "funct7": 0b0000000,
  },
  "sll": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b001,
    "funct7": 0b0000000,
  },
  "srl": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b101,
    "funct7": 0b0000000,
  },
  "sra": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b101,
    "funct7": 0b0100000,
  },
  "slt": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b010,
    "funct7": 0b0000000,
  },
  "sltu": {
    "type"  : "R",
    "opcode": 0b0110011,
    "funct3": 0b011,
    "funct7": 0b0000000,
  },

  # ---------------- I ----------------
  "addi": {
    "type"  : "I",
    "opcode": 0b0010011,
    "funct3": 0b000,
  },
  "andi": {
    "type"  : "I",
    "opcode": 0b0010011,
    "funct3": 0b111,
  },
  "ori": {
    "type"  : "I",
    "opcode": 0b0010011,
    "funct3": 0b110,
  },
  "xori": {
    "type"  : "I",
    "opcode": 0b0010011,
    "funct3": 0b100,
  },
  "slti": {
    "type"  : "I",
    "opcode": 0b0010011,
    "funct3": 0b010,
  },
  "sltiu": {
    "type"  : "I",
    "opcode": 0b0010011,
    "funct3": 0b011,
  },
  "lw": {
    "type"  : "I",
    "opcode": 0b0000011,
    "funct3": 0b010,
  },
  "jalr": {
    "type"  : "I",
    "opcode": 0b1100111,
    "funct3": 0b000,
  },

  # ---------------- S ----------------
  "sw": {
    "type"  : "S",
    "opcode": 0b0100011,
    "funct3": 0b010,
  },

  # ---------------- B ----------------
  "beq": {
    "type"  : "B",
    "opcode": 0b1100011,
    "funct3": 0b000,
  },
  "bne": {
    "type"  : "B",
    "opcode": 0b1100011,
    "funct3": 0b001,
  },
  "blt": {
    "type"  : "B",
    "opcode": 0b1100011,
    "funct3": 0b100,
  },
  "bge": {
    "type"  : "B",
    "opcode": 0b1100011,
    "funct3": 0b101,
  },
  "bltu": {
    "type"  : "B",
    "opcode": 0b1100011,
    "funct3": 0b110,
  },
  "bgeu": {
    "type"  : "B",
    "opcode": 0b1100011,
    "funct3": 0b111,
  },

  # ---------------- U ----------------
  "lui": {
    "type"  : "U",
    "opcode": 0b0110111,
  },
  "auipc": {
    "type"  : "U",
    "opcode": 0b0010111,
  },

  # ---------------- J ----------------
  "jal": {
    "type"  : "J",
    "opcode": 0b1101111,
  },
}
