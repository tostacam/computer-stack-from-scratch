instruction_table = {
  "add" : {
    "opcode": 0b0110011, 
    "funct3": 0b000,
    "funct7": 0b0000000
  },
  "sub" : {
    "opcode": 0b0110011,
    "funct3": 0b000,
    "funct7": 0b0100000 
  }
}

def register_number(register):
  return int(register[1:])

def read_file(filename):
  with open(filename, "r") as file:
    lines = file.readlines()

  return [line.strip() for line in lines]

def tokenize(program):
  return [program_line.replace(",", "").split() for program_line in program]

def parse(tokens):
  instructions = []

  for token in tokens:
    if token[0] == "add":
      instruction = {
        "mnemonic": "add",
        "rd": register_number(token[1]),
        "rs1": register_number(token[2]),
        "rs2": register_number(token[3])
      }
    elif token[0] == "sub":
      instruction = {
        "mnemonic": "sub",
        "rd": register_number(token[1]),
        "rs1": register_number(token[2]),
        "rs2": register_number(token[3])
      }
    instructions.append(instruction)

  return instructions

def encode(instructions):
  machine_code = []

  for instruction in instructions:
    isa = instruction_table[instruction["mnemonic"]]

    opcode = isa["opcode"]
    funct3 = isa["funct3"]
    funct7 = isa["funct7"]

    rd  = instruction["rd"]
    rs1 = instruction["rs1"]
    rs2 = instruction["rs2"]

    encoded_instruction = 0
    encoded_instruction |= opcode
    encoded_instruction |= rd << 7
    encoded_instruction |= funct3 << 12
    encoded_instruction |= rs1 << 15
    encoded_instruction |= rs2 << 20
    encoded_instruction |= funct7 << 25

    machine_code.append(hex(encoded_instruction))

  return machine_code

def write_file(filename, machine_code):
  print("write file")

def main():
  filename = "test.s"

  program = read_file(filename)
  tokens = tokenize(program)
  instructions = parse(tokens)
  machine_code = encode(instructions)
  write_file(filename, machine_code);

  print(machine_code)

if __name__ == "__main__":
  main()
