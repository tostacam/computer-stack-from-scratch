from instruction_table import instruction_table

def register_number(register):
  return int(register[1:])

def read_file(input_filename):
  with open(input_filename, "r") as file:
    lines = file.readlines()

  return [line.strip() for line in lines]

def tokenize(program):
  return [program_line.replace(",", "").split() for program_line in program]

def parse(tokens):
  instructions = []

  for token in tokens:
    mnemonic = token[0]
    instruction_type = instruction_table[mnemonic]["type"]

    if instruction_type == "R":
      instruction = {
        "type"    : "R",
        "mnemonic": mnemonic,
        "rd"      : register_number(token[1]),
        "rs1"     : register_number(token[2]),
        "rs2"     : register_number(token[3])
      }
    elif instruction_type == "I":
      instruction = {
        "type"    : "I",
        "mnemonic": mnemonic,
        "rs1"     : register_number(token[1]),
        "rs2"     : register_number(token[2]),
        "imm"     : register_number(token[3])
      }
    elif instruction_type == "S":
      instruction = {
        "type"    : "S",
        "mnemonic": mnemonic,
        "rs1"     : register_number(token[1]),
        "rs2"     : register_number(token[2]),
        "imm"     : token[3]
      }
    elif instruction_type == "B":
      instruction = {
        "type"    : "B",
        "mnemonic": mnemonic,
        "rs1"     : register_number(token[1]),
        "rs2"     : register_number(token[2]),
        "label"   : token[3]
      }
    elif instruction_type == "U":
      instruction = {
        "type"    : "U",
        "mnemonic": mnemonic,
        "rd"      : register_number(token[1]),
        "imm"     : int(token[2])
      }
    elif instruction_type == "J":
      instruction = {
        "type"    : "J",
        "mnemonic": mnemonic,
        "rd"      : register_number(token[1]),
        "label"   : token[2]
      }

    instructions.append(instruction)

  return instructions

def encode(instructions):
  machine_code = []

  for instruction in instructions:
    isa_code = instruction_table[instruction["mnemonic"]]
    encoded_instruction = 0

    if instruction["type"] == "R":
      opcode  = isa_code["opcode"] 
      rd      = instruction["rd"]
      funct3  = isa_code["funct3"]
      rs1     = instruction["rs1"]
      rs2     = instruction["rs2"]
      funct7  = isa_code["funct7"]

      encoded_instruction = (
        opcode
        | (rd << 7)
        | (funct3 << 12)
        | (rs1 << 15)
        | (rs2 << 20)
        | (funct7 << 25)
      )
    elif instruction["type"] == "I":
      opcode  = isa_code["opcode"]
      rd      = instruction["rd"]
      funct3  = isa_code["funct3"]
      rs1     = instruction["rs1"]
      imm     = instruction["imm"]

      encoded_instruction = (
        opcode
        | (rd << 7)
        | (funct3 << 12)
        | (rs1 << 15)
        | (imm << 20)
      )
    elif instruction["type"] == "S":
      opcode  = isa_code["opcode"]
      funct3  = isa_code["funct3"]
      rs1     = instruction["rs1"]
      rs2     = instruction["rs2"]
      imm     = instruction["imm"]
      
      imm_low  = imm         & 0x1F # bits 4:0
      imm_high = (immm >> 5) & 0x7F # bits 11:5

      encoded_instruction = (
        opcode
        | (imm_low << 7)
        | (funct3 << 12)
        | (rs1 << 15)
        | (rs2 << 20)
        | (imm_high << 25)
      )
    elif instruction["type"] == "B":
      opcode  = isa_code["opcode"]
      funct3  = isa_code["funct3"]
      rs1     = instruction["rs1"]
      rs2     = instruction["rs2"]
      imm     = instruction["imm"]

      imm_12    = (imm >> 12) & 0x1   # bits 12
      imm_10_5  = (imm >> 5)  & 0x3F  # bits 10:5
      imm_4_1   = (imm >> 1)  & 0xF   # bits 4:1
      imm_11    = (imm >> 11) & 0x1   # bits 11

      encoded_instruction = (
        opcode
        | (imm_11 << 7)
        | (imm_4_1 << 8)
        | (funct3 << 12)
        | (rs1 << 15)
        | (rs2 << 20)
        | (imm_10_5 << 25)
        | (imm_12 << 31)
      )
    elif instruction["type"] == "U":
      opcode  = isa_code["opcode"]
      rd      = instruction["rd"]
      imm     = instruction["imm"]

      encoded_instruction = (
        opcode
        | (rd << 7)
        | (imm << 12)
      )
    elif instruction["type"] == "J":
      opcode  = isa_code["opcode"]
      rd      = instruction["rd"]
      imm     = instruction["imm"]

      imm_20    = (imm >> 20) & 0x1   # bits 20
      imm_10_1  = (imm >> 1)  & 0x3FF # bits 10:1
      imm_11    = (imm >> 11) & 0x1   # bits 11
      imm_19_12 = (imm >> 12) & 0xFF  # bits 19:12

      encoded_instruction = (
        opcode
        | (rd << 7)
        | (imm_19_12 << 12)
        | (imm_11 << 20)
        | (imm_10_1 << 21)
        | (imm_20 << 31)
      )

    machine_code.append(encoded_instruction)
  return machine_code

def write_file(output_filename, machine_code):
  with open(output_filename, "w") as file:
    for instruction in machine_code:
      file.write(f"{instruction:08X}\n")

def main():
  input_filename = "test.s"
  output_filename = "test.hex"
  program = read_file(input_filename)
  tokens = tokenize(program)
  instructions = parse(tokens)
  machine_code = encode(instructions)
  write_file(output_filename, machine_code);

if __name__ == "__main__":
  main()
