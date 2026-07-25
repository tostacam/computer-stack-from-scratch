#include "cpu.h"

static void CPU_init(CPU *cpu, ROM *rom) {
  program_counter_init(&cpu->pc);
  cpu->rom = *rom;
  register_file_init(&cpu->rf);
  alu_init(&cpu->alu);
  RAM_init(&cpu->ram);

  cpu->instruction        = bus64_zero();
  cpu->jump_address       = bus64_zero();
  cpu->control.alu_src    = 0;
  cpu->control.mem_to_reg = 0;
  cpu->control.reg_write  = 0;
  cpu->control.mem_read   = 0;
  cpu->control.mem_write  = 0;
  cpu->control.branch     = 0;
  cpu->control.branch_neq = 0;
  cpu->control.alu_op     = 0;
}

static void fetch(CPU *cpu) {
  uint64_t fetch_address = decode_amount(register64_output(&cpu->pc.output_reg));
  uint32_t instr = (ROM_read(&cpu->rom, fetch_address)) | \
                   (ROM_read(&cpu->rom, fetch_address + 1) << 8) | \
                   (ROM_read(&cpu->rom, fetch_address + 2) << 16) | \
                   (ROM_read(&cpu->rom, fetch_address + 3) << 24);
  cpu->instruction = encode_amount((uint64_t)instr);
}

static enum alu_op alu_control(uint8_t alu_op, uint8_t funct3, uint8_t funct7) {
  switch (alu_op) {
    // 000 -> always ADD
    case 0b000:
      return ALU_OP_ADD;

    // 001 -> R-type decode
    case 0b001:
      switch (funct3) {
        case 0b000: return (funct7 == 0b0100000) ? ALU_OP_SUB : ALU_OP_ADD;
        case 0b001: return ALU_OP_SLL;
        case 0b010: return ALU_OP_SLT;
        case 0b011: return ALU_OP_SLTU;
        case 0b100: return ALU_OP_XOR;
        case 0b101: return (funct7 == 0b0100000) ? ALU_OP_SRA : ALU_OP_SRL;
        case 0b110: return ALU_OP_OR;
        case 0b111: return ALU_OP_AND;
      }
      
    // 010 -> BRANCH compare
    case 0b010:
      switch (funct3) {
        case 0b000:
        case 0b001: return ALU_OP_SUB;
        case 0b100:
        case 0b101: return ALU_OP_SLT;
        case 0b110:
        case 0b111: return ALU_OP_SLTU;
      }

    // 011 -> I-type decode
    case 0b011:
      switch (funct3) {
        case 0b000: return ALU_OP_ADD;
        case 0b001: return ALU_OP_SLL;
        case 0b010: return ALU_OP_SLT;
        case 0b011: return ALU_OP_SLTU;
        case 0b100: return ALU_OP_XOR;
        case 0b101: return (funct7 == 0b0100000) ? ALU_OP_SRA : ALU_OP_SRL;
        case 0b110: return ALU_OP_OR;
        case 0b111: return ALU_OP_AND;
      }

    // 100 -> LUI
    case 0b100:
      return ALU_OP_PASS_B;

    // 101 -> AUIPC
    case 0b101:
      return ALU_OP_ADD;

    // 110 -> JAL/JALR
    case 0b110:
      return ALU_OP_ADD;

    // default
    default:
      return ALU_OP_ADD;
  }
}

static bus64 immediate_generator(bus64 instruction) {
  // opcode
  uint64_t opcode = decode_nbits(cpu->instruction, 0, 6);
  uint64_t immediate;

  switch (opcode) {
    case OPCODE_LOAD:
    case OPCODE_ITYPE:
    case OPCODE_JALR: 
      immediate = decode_nbits(cpu->instruction, 20, 31);
      break;
    case OPCODE_STORE: 
      immediate = (decode_nbits(cpu->instruction, 25, 31) << 5) | decode_nbits(cpu->instruction, 7, 11); 
      break;
    case OPCODE_BRANCH:
      immediate =
        (decode_nbits(cpu->instruction, 31, 31) << 12) |
        (decode_nbits(cpu->instruction, 7, 7)   << 11) |
        (decode_nbits(cpu->instruction, 25, 30) << 5)  |
        (decode_nbits(cpu->instruction, 8, 11)  << 1);
      break;
    case OPCODE_LUI:
    case OPCODE_AUIPC:
      immediate = decode_nbits(cpu->instruction, 12, 31);
      break;
    case OPCODE_JAL:
      immediate = 
        (decode_nbits(cpu->instruction, 31, 31) << 20) |
        (decode_nbits(cpu->instruction, 12, 19) << 12) |
        (decode_nbits(cpu->instruction, 20, 20) << 11) |
        (decode_nbits(cpu->instruction, 21, 30) << 1);
      break;
    default:
      immediate = 0;
      break;
  }

  return encode_amount(immediate);
}
  
static void decode(CPU *cpu) {
  // opcode
  uint64_t opcode = decode_nbits(cpu->instruction, 0, 6);

  // Read Registers / Write Register Address
  cpu->rf.read_addr_a = encode_amount(rs1);
  cpu->rf.read_addr_b = encode_amount(rs2);
  register_file_eval(&cpu->rf);
  cpu->rf.write_addr  = encode_amount(rd);

  // default control_signals
  cpu->control.alu_src    = 0;
  cpu->control.mem_to_reg = 0;
  cpu->control.reg_write  = 0;
  cpu->control.mem_read   = 0;
  cpu->control.mem_write  = 0;
  cpu->control.branch     = 0;
  cpu->control.branch_neq = 0;
  cpu->control.alu_op     = 0;

  switch (opcode) {
    case OPCODE_RTYPE: 
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b001;
      break;
    case OPCODE_ITYPE:
      cpu->control.alu_src    = 1;
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b011;
      break;
    case OPCODE_LOAD:
      cpu->control.alu_src    = 1;
      cpu->control.mem_to_reg = 1;
      cpu->control.reg_write  = 1;
      cpu->control.mem_read   = 1;
      cpu->control.alu_op     = 0b000;
      break;
    case OPCODE_STORE:
      cpu->control.mem_write  = 1;
      cpu->control.alu_src    = 1;
      cpu->control.alu_op     = 0b000;
      break;
    case OPCODE_BRANCH:
      cpu->control.branch     = 1;
      cpu->control.alu_op     = 0b010;
      break;
    case OPCODE_LUI:
      cpu->control.alu_src    = 1;
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b100;
      break;
    case OPCODE_AUIPC:
      cpu->control.alu_src    = 1;
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b101;
      break;
    case OPCODE_JAL:
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b110;
      break;
    case OPCODE_JALR:
      cpu->control.alu_src    = 1;
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b110;
      break;
    case OPCODE_SYSTEM:
      cpu->control.alu_op     = 0b111;
      break;
  }

  cpu->alu.opcode = alu_control(cpu->control.alu_op, funct3, funct7);
  cpu->alu.a      = cpu->rf.read_data_a;
  cpu->alu.b      = cpu->control.alu_src ? immediate_generator(cpu->instruction) : rf.read_data_b; 

  if (cpu->control.alu_src == 0) {
    cpu->alu.b = cpu->rf.read_data_b;
  } else {
    switch (opcode) { // pg. 126, COD: RISC-V
      case 0b0010011: // I-type (addi)
      case 0b0000011: // I-type (lw)
        cpu->alu.b = encode_amount(imm_I_bits);
        break;
      case 0b0100011: // S-type (sw)
        cpu->alu.b = encode_amount(imm_S_bits);
        break;
      case 0b1100011: // SB-type
        cpu->alu.b = encode_amount(imm_SB_bits);
        break;
      case 0b0110111: // U-type (lui)
        cpu->alu.b = encode_amount(imm_U_bits);
        break;
      case 0b1101111: // UJ-type (jal)
        cpu->alu.b = encode_amount(imm_UJ_bits);
        break;
    }
  }

  if (opcode == 0b1100011) { // SB
    cpu->jump_address = add64_no_crry(register64_output(&cpu->pc.output_reg), encode_amount(imm_SB_bits));
  }
  if (opcode == 0b1101111) { // JAL
    cpu->jump_address = add64_no_crry(register64_output(&cpu->pc.output_reg), encode_amount(imm_UJ_bits));
  }
}

static void execute(CPU *cpu) {
  alu_eval(&cpu->alu);
}

static void memory_access(CPU *cpu) {
  assert(!(cpu->ram.read_enable && cpu->ram.write_enable));

  cpu->ram.address      = decode_amount(cpu->alu.output);
  cpu->ram.write_data   = decode_amount(cpu->rf.read_data_b);
  cpu->ram.write_enable = cpu->control.mem_write;
  cpu->ram.read_enable  = cpu->control.mem_read;

  RAM_read(&cpu->ram, MEM_WORD_SIZE);
  RAM_write(&cpu->ram, MEM_DWRD_SIZE);
}

static void write_back(CPU *cpu) {
  cpu->rf.write_enable = cpu->control.reg_write;
  cpu->rf.write_data = cpu->control.mem_to_reg ? encode_amount(cpu->ram.read_data) : cpu->alu.output;
  register_file_tick(&cpu->rf);
}

static void update_pc(CPU *cpu) {
  cpu->pc.jump_addr = cpu->jump_address;
  if (cpu->control.branch) {
    cpu->pc.jump = cpu->control.branch_neq ? !cpu->alu.f_zero : cpu->alu.f_zero;
  }
  program_counter_tick(&cpu->pc);
}

void CPU_cycle(CPU *cpu) {
  fetch(cpu);
  decode(cpu);
  execute(cpu);
  memory_access(cpu);
  write_back(cpu);
  update_pc(cpu);
}
