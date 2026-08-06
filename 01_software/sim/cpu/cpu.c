#include "cpu.h"

void CPU_init(CPU *cpu, ROM *rom) {
  program_counter_init(&cpu->pc);
  cpu->rom = *rom;
  register_file_init(&cpu->rf);
  alu_init(&cpu->alu);
  RAM_init(&cpu->ram);

  cpu->instruction        = bus64_zero();
  cpu->immediate          = bus64_zero();  
  cpu->jump_address       = bus64_zero();
  cpu->control.alu_src_a  = 0;
  cpu->control.alu_src_b  = 0;
  cpu->control.wb_src     = 0;
  cpu->control.reg_write  = 0;
  cpu->control.mem_read   = 0;
  cpu->control.mem_write  = 0;
  cpu->control.pc_src     = 0;
  cpu->control.alu_op     = 0;
  cpu->state              = CPU_RUNNING;
  cpu->trap               = TRAP_NONE;
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

    // 111 -> SYSTEM
    case 0b111:
      return ALU_OP_PASS_B;

    // default
    default:
      return ALU_OP_ADD;
  }
}

static uint64_t sign_extended_imm(uint64_t imm_value, unsigned bits) {
  uint64_t sign = 1ULL << (bits - 1);

  if (imm_value & sign) {
    imm_value |= (~0ULL << bits);
  }

  return imm_value;
}

static bus64 immediate_generator(bus64 instruction) {
  // opcode
  uint64_t opcode = decode_nbits(instruction, 0, 6);
  uint64_t immediate;

  switch (opcode) {
    case OPCODE_LOAD:
    case OPCODE_ITYPE:
    case OPCODE_JALR: 
      immediate = decode_nbits(instruction, 20, 31);
      immediate = sign_extended_imm(immediate, 12);
      break;
    case OPCODE_STORE: 
      immediate = (decode_nbits(instruction, 25, 31) << 5) | decode_nbits(instruction, 7, 11);
      immediate = sign_extended_imm(immediate, 12);
      break;
    case OPCODE_BRANCH:
      immediate =
        (decode_nbits(instruction, 31, 31) << 12) |
        (decode_nbits(instruction, 7, 7)   << 11) |
        (decode_nbits(instruction, 25, 30) << 5)  |
        (decode_nbits(instruction, 8, 11)  << 1);
      immediate = sign_extended_imm(immediate, 13);
      break;
    case OPCODE_LUI:
    case OPCODE_AUIPC:
      immediate = decode_nbits(instruction, 12, 31) << 12;
      break;
    case OPCODE_JAL:
      immediate = 
        (decode_nbits(instruction, 31, 31) << 20) |
        (decode_nbits(instruction, 12, 19) << 12) |
        (decode_nbits(instruction, 20, 20) << 11) |
        (decode_nbits(instruction, 21, 30) << 1);
      immediate = sign_extended_imm(immediate, 21);
      break;
    default:
      immediate = 0;
      break;
  }

  return encode_amount(immediate);
}
  
static void decode(CPU *cpu) {
  // opcode, regsiters, funct3/7
  uint64_t opcode = decode_nbits(cpu->instruction, 0, 6);
  uint64_t rd     = decode_nbits(cpu->instruction, 7, 11);
  uint64_t funct3 = decode_nbits(cpu->instruction, 12, 14);
  uint64_t rs1    = decode_nbits(cpu->instruction, 15, 19);
  uint64_t rs2    = decode_nbits(cpu->instruction, 20, 24);
  uint64_t funct7 = decode_nbits(cpu->instruction, 25, 31);

  // Read Registers / Write Register Address
  cpu->rf.read_addr_a = encode_amount(rs1);
  cpu->rf.read_addr_b = encode_amount(rs2);
  register_file_eval(&cpu->rf);
  cpu->rf.write_addr  = encode_amount(rd);

  // default control_signals
  cpu->control.alu_src_a  = 0;
  cpu->control.alu_src_b  = 0;
  cpu->control.wb_src     = WB_ALU;
  cpu->control.reg_write  = 0;
  cpu->control.mem_read   = 0;
  cpu->control.mem_write  = 0;
  cpu->control.pc_src     = PC_NEXT;
  cpu->control.alu_op     = 0;

  switch (opcode) {
    case OPCODE_RTYPE: 
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b001;
      break;
    case OPCODE_ITYPE:
      cpu->control.alu_src_b  = 1;
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b011;
      break;
    case OPCODE_LOAD:
      cpu->control.wb_src     = WB_MEM;
      cpu->control.alu_src_b  = 1;
      cpu->control.reg_write  = 1;
      cpu->control.mem_read   = 1;
      cpu->control.alu_op     = 0b000;
      break;
    case OPCODE_STORE:
      cpu->control.wb_src     = WB_MEM;
      cpu->control.mem_write  = 1;
      cpu->control.alu_src_b  = 1;
      cpu->control.alu_op     = 0b000;
      break;
    case OPCODE_BRANCH:
      cpu->control.pc_src     = PC_BRANCH;
      cpu->control.alu_op     = 0b010;
      break;
    case OPCODE_LUI:
      cpu->control.alu_src_a  = 1;
      cpu->control.alu_src_b  = 1;
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b100;
      break;
    case OPCODE_AUIPC:
      cpu->control.alu_src_a  = 1;
      cpu->control.alu_src_b  = 1;
      cpu->control.reg_write  = 1;
      cpu->control.alu_op     = 0b101;
      break;
    case OPCODE_JAL:
      cpu->control.wb_src     = WB_PC4;
      cpu->control.reg_write  = 1;
      cpu->control.pc_src     = PC_JAL;
      cpu->control.alu_op     = 0b110;
      break;
    case OPCODE_JALR:
      cpu->control.wb_src     = WB_PC4;
      cpu->control.alu_src_b  = 1;
      cpu->control.reg_write  = 1;
      cpu->control.pc_src     = PC_JALR;
      cpu->control.alu_op     = 0b110;
      break;
    case OPCODE_SYSTEM:
      cpu->control.alu_src_b  = 1;
      cpu->control.alu_op     = 0b111;
      break;
  }

  cpu->immediate  = immediate_generator(cpu->instruction);

  cpu->alu.opcode = alu_control(cpu->control.alu_op, funct3, funct7);
  cpu->alu.a      = cpu->control.alu_src_a ? register64_output(&cpu->pc.output_reg) : cpu->rf.read_data_a;
  cpu->alu.b      = cpu->control.alu_src_b ? cpu->immediate : cpu->rf.read_data_b; 

  cpu->jump_address = add64_no_crry(register64_output(&cpu->pc.output_reg), cpu->immediate);
}

static void execute(CPU *cpu) {
  alu_eval(&cpu->alu);

  uint64_t opcode = decode_nbits(cpu->instruction, 0, 6);
  if (opcode == OPCODE_SYSTEM) {
    switch (decode_amount(cpu->immediate)) {
      case 0:
        cpu->trap = TRAP_ECALL;
        return;
      case 1:
        cpu->trap = TRAP_EBREAK;
        return;
    }
  }
}

static void memory_access(CPU *cpu) {
  assert(!(cpu->ram.read_enable && cpu->ram.write_enable));

  cpu->ram.address      = decode_amount(cpu->alu.output);
  cpu->ram.write_data   = decode_amount(cpu->rf.read_data_b);
  cpu->ram.write_enable = cpu->control.mem_write;
  cpu->ram.read_enable  = cpu->control.mem_read;

  uint64_t funct3 = decode_nbits(cpu->instruction, 12, 14);
  enum MEM_SIZE RAM_MEM_SIZE;
  switch (funct3) {
    case 0b000: RAM_MEM_SIZE = MEM_BYTE;  break;  // lb
    case 0b001: RAM_MEM_SIZE = MEM_HALF;  break;  // lh
    case 0b010: RAM_MEM_SIZE = MEM_WORD;  break;  // lw
    case 0b011: RAM_MEM_SIZE = MEM_DWRD;  break;  // ld
    case 0b100: RAM_MEM_SIZE = MEM_UBYTE; break;  // lbu
    case 0b101: RAM_MEM_SIZE = MEM_UHALF; break;  // lhu
    case 0b110: RAM_MEM_SIZE = MEM_UWORD; break;  // lwu
    default:    RAM_MEM_SIZE = MEM_WORD;  break;
  }

  RAM_read(&cpu->ram, RAM_MEM_SIZE);
  RAM_write(&cpu->ram, RAM_MEM_SIZE);
}

static void write_back(CPU *cpu) {
  cpu->rf.write_enable = cpu->control.reg_write;
  switch (cpu->control.wb_src) {
    case WB_ALU:
      cpu->rf.write_data = cpu->alu.output;
      break;
    case WB_MEM:
      cpu->rf.write_data = encode_amount(cpu->ram.read_data);
      break;
    case WB_PC4:
      cpu->rf.write_data = add64_no_crry(register64_output(&cpu->pc.output_reg), encode_amount(4));
      break;
  }
  register_file_tick(&cpu->rf);
}

static void update_pc(CPU *cpu) {
  cpu->pc.jump_addr = cpu->jump_address;
  uint8_t funct3 = decode_nbits(cpu->instruction, 12, 14);

  switch (cpu->control.pc_src) {
    case PC_NEXT:
      cpu->pc.jump = 0;
      break;
    case PC_BRANCH:
      switch (funct3) {
        case FUNCT3_BEQ:
          cpu->pc.jump = cpu->alu.f_zero;
          break;
        case FUNCT3_BNE:
          cpu->pc.jump = !cpu->alu.f_zero;
          break;
        case FUNCT3_BLT:
          cpu->pc.jump = (decode_amount(cpu->alu.output) == 1);
          break;
        case FUNCT3_BGE:
          cpu->pc.jump = (decode_amount(cpu->alu.output) == 0);
          break;
        case FUNCT3_BLTU:
          cpu->pc.jump = (decode_amount(cpu->alu.output) == 1);
          break;
        case FUNCT3_BGEU:
          cpu->pc.jump = (decode_amount(cpu->alu.output) == 0);
          break;
      }
      break;
    case PC_JAL:
      cpu->pc.jump = 1;
      cpu->pc.jump_addr = add64_no_crry(register64_output(&cpu->pc.output_reg), cpu->immediate);
      break;
    case PC_JALR:
      cpu->pc.jump = 1;
      cpu->pc.jump_addr =
        bitwise_and(add64_no_crry(cpu->rf.read_data_a, cpu->immediate), encode_amount(~1ULL));
      break;
  } 
  
  program_counter_tick(&cpu->pc);
}

void CPU_cycle(CPU *cpu) {
  if (cpu->state == CPU_HALTED)
    return;

  fetch(cpu);
  decode(cpu);
  execute(cpu);

  if (cpu->trap != TRAP_NONE) {
    cpu->state = CPU_HALTED;
    return;
  }

  memory_access(cpu);
  write_back(cpu);
  update_pc(cpu);
}
