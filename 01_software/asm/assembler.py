import sys
from instruction_table import instruction_table

SECTION_BASES = {
  ".text":    0x000,
  ".data":    0x100,
  ".rodata":  0x200,
  ".bss":     0x300,
}

def register_number(register):
  return int(register[1:])

def read_file(input_filename):
  with open(input_filename, "r") as file:
    lines = file.readlines()

  program = []

  for line in lines:
    line = line.split("#")[0]
    line = line.strip()

    if not line:
      continue

    program.append(line)

  return program

def tokenize(program):
  tokens = []

  for line in program:
    line = line.replace(",", " ")
    line = line.replace("(", " ")
    line = line.replace(")", " ")
    tokens.append(line.split())

  return tokens

def first_pass(tokens):
  symbol_table = {}
  section_offsets = {section: 0 for section in SECTION_BASES}
  section_current = ".text"

  for token in tokens:
    # defintion / constant
    if len(token) == 3 and token[1] == "=":
      symbol_table[token[0]] = int(token[2], 0)
      continue

    # section change
    if len(token) == 1 and token[0] in SECTION_BASES:
      section_current = token[0]
      continue

    # labels within section
    if token[0].endswith(":"):
      symbol_table[token[0].strip(":")] = (
        SECTION_BASES[section_current]
        + section_offsets[section_current]
      )

      token = token[1:]

      if not token:
        continue

    # data types
    if token[0] == ".word": 
      section_offsets[section_current] += 4
    elif token[0] == ".half":
      section_offsets[section_current] += 2
    elif token[0] == ".byte":
      section_offsets[section_current] += 1
    elif token[0] == ".space":
      section_offsets[section_current] += int(token[1], 0)
    else:
      section_offsets[section_current] += 4

  return symbol_table

def resolve_value(value, symbol_table):
  if value in symbol_table:
    return symbol_table[value]

  return int(value, 0)

def parse(tokens, symbol_table):
  instructions = []
  section_current = ".text"

  for token in tokens:
    # section change
    if len(token) == 1 and token[0] in SECTION_BASES:
      section_current = token[0]
      continue

    # definition / constant
    if len(token) == 3 and token[1] == "=":
      continue

    # labels within section
    if token[0].endswith(":"):
      token = token[1:]

      if not token:
        continue

    # data types
    if token[0] in [".word", ".half", ".byte", ".space"]:
      continue

    if section_current != ".text":
      raise ValueError(
        f"no instruction allowed outside of .text for now"
      )

    mnemonic = token[0]
    isa_data = instruction_table[mnemonic]
    instruction_type = isa_data["type"]

    if instruction_type == "R":
      instruction = {
        "type"    : "R",
        "mnemonic": mnemonic,
        "rd"      : register_number(token[1]),
        "rs1"     : register_number(token[2]),
        "rs2"     : register_number(token[3])
      }
    elif instruction_type == "I":
      if isa_data["I-type"] == "arithmetic":
       instruction = {
          "type"    : "I",
          "mnemonic": mnemonic,
          "rd"      : register_number(token[1]),
          "rs1"     : register_number(token[2]),
          "imm"     : resolve_value(token[3], symbol_table)
        }
      elif isa_data["I-type"] == "offset":
        instruction = {
          "type"    : "I",
          "mnemonic": mnemonic,
          "rd"      : register_number(token[1]),
          "rs1"     : register_number(token[3]),
          "imm"     : resolve_value(token[2], symbol_table)
        }
      elif isa_data["I-type"] == "system":
        instruction = {
          "type"    : "I",
          "mnemonic": mnemonic,
          "rd"      : 0,
          "rs1"     : 0,
          "imm"     : isa_data["imm"]
        }
    elif instruction_type == "S":
      instruction = {
        "type"    : "S",
        "mnemonic": mnemonic,
        "rs2"     : register_number(token[1]),
        "imm"     : resolve_value(token[2], symbol_table),
        "rs1"     : register_number(token[3])
      }
    elif instruction_type == "B":
      instruction = {
        "type"    : "B",
        "mnemonic": mnemonic,
        "rs1"     : register_number(token[1]),
        "rs2"     : register_number(token[2]),
        "label"   : symbol_table[token[3]]
      }
    elif instruction_type == "U":
      instruction = {
        "type"    : "U",
        "mnemonic": mnemonic,
        "rd"      : register_number(token[1]),
        "imm"     : resolve_value(token[2], symbol_table)
      }
    elif instruction_type == "J":
      instruction = {
        "type"    : "J",
        "mnemonic": mnemonic,
        "rd"      : register_number(token[1]),
        "label"   : symbol_table[token[2]]
      }

    instructions.append(instruction)

  return instructions

def encode(instructions, symbol_table):
  machine_code = []
  pc = 0

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

      if instruction["mnemonic"] == "srai":
        imm |= 0b0100000 << 5

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
      
      imm_low  = imm        & 0x1F  # bits 4:0
      imm_high = (imm >> 5) & 0x7F  # bits 11:5

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
      label   = instruction["label"]
      
      imm = label - pc
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
      label   = instruction["label"]

      imm = label - pc
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
    pc += 4
  return machine_code

def write_file(output_filename, machine_code):
  with open(output_filename, "w") as file:
    for instruction in machine_code:
      file.write(f"{instruction & 0xFFFFFFFF:08x}\n")

def main(input_filename, output_filename):
  program       = read_file(input_filename)
  tokens        = tokenize(program)
  symbol_table  = first_pass(tokens)
  instructions  = parse(tokens, symbol_table)
  machine_code  = encode(instructions, symbol_table)
  write_file(output_filename, machine_code);

if __name__ == "__main__":
  if len(sys.argv) != 3:
    print(f"Need input/output files: python3 {sys.argv[0]} <input.s> <output.hex>")
    sys.exit(1)
  main(sys.argv[1], sys.argv[2])
