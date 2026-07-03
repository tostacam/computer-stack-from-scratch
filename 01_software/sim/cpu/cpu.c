#include "cpu.h"

void CPU_init(CPU *cpu, ROM *rom) {
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
  cpu->control.alu_op[0]  = 0;
  cpu->control.alu_op[1]  = 0;
}

static void fetch(CPU *cpu) {
  uint64_t fetch_address = decode_amount(register64_output(&cpu->pc.output_reg));
  uint32_t instr = (ROM_read(&cpu->rom, fetch_address)) | \
                   (ROM_read(&cpu->rom, fetch_address + 1) << 8) | \
                   (ROM_read(&cpu->rom, fetch_address + 2) << 16) | \
                   (ROM_read(&cpu->rom, fetch_address + 3) << 24);
  cpu->instruction = encode_amount((uint64_t)instr);
}

static enum alu_op alu_control(bit alu_op[2], uint8_t funct3, uint8_t funct7) {
  // 00 -> always ADD
  if (alu_op[1] == 0 && alu_op[0] == 0)
    return ALU_OP_ADD;

  // 01 -> always SUB (beq/bne compare)
  if (alu_op[1] == 0 && alu_op[0] == 1)
    return ALU_OP_SUB;

  // 10 -> R-type
  if (alu_op[1] == 1 && alu_op[0] == 0) {
    // add
    if (funct3 == 0b000 && funct7 == 0b0000000)
      return ALU_OP_ADD;

    // sub
    if (funct3 == 0b000 && funct7 == 0b0100000)
      return ALU_OP_SUB;
  }

  return ALU_OP_ADD;
}

static void decode(CPU *cpu) {
  // opcode
  uint64_t opcode = decode_nbits(cpu->instruction, 0, 6);
  
  // R-type
  uint64_t rd     = decode_nbits(cpu->instruction, 7, 11);
  uint64_t funct3 = decode_nbits(cpu->instruction, 12, 14);
  uint64_t rs1    = decode_nbits(cpu->instruction, 15, 19);
  uint64_t rs2    = decode_nbits(cpu->instruction, 20, 24);
  uint64_t funct7 = decode_nbits(cpu->instruction, 25, 31);
  // I-type
  uint64_t imm_I_bits = decode_nbits(cpu->instruction, 20, 31);
  // S-type
  uint64_t imm_S_bits = (decode_nbits(cpu->instruction, 25, 31) << 5) | decode_nbits(cpu->instruction, 7, 11);
  // SB-type
  uint64_t imm_SB_bits =
    (decode_nbits(cpu->instruction, 31, 31) << 12) |
    (decode_nbits(cpu->instruction, 7, 7)   << 11) |
    (decode_nbits(cpu->instruction, 25, 30) << 5)  |
    (decode_nbits(cpu->instruction, 8, 11)  << 1);
  // U-type
  uint64_t imm_U_bits = decode_nbits(cpu->instruction, 12, 31);
  // UJ-type
  uint64_t imm_UJ_bits =
    (decode_nbits(cpu->instruction, 31, 31) << 20) |
    (decode_nbits(cpu->instruction, 12, 19) << 12) |
    (decode_nbits(cpu->instruction, 20, 20) << 11) |
    (decode_nbits(cpu->instruction, 21, 30) << 1);

  // Read Registers / Write Register Address
  cpu->rf.read_addr_a = encode_amount(rs1);
  cpu->rf.read_addr_b = encode_amount(rs2);
  register_file_eval(&cpu->rf);
  cpu->rf.write_addr  = encode_amount(rd);

  // R-type
  if (opcode == 0b0110011) {
    if (funct3 == 0b000) {
      if (funct7 == 0b0000000) { // add
        cpu->control.alu_src    = 0;
        cpu->control.mem_to_reg = 0;
        cpu->control.reg_write  = 1;
        cpu->control.mem_read   = 0;
        cpu->control.mem_write  = 0;
        cpu->control.branch     = 0;
        cpu->control.branch_neq = 0;
        cpu->control.alu_op[0]  = 0;
        cpu->control.alu_op[1]  = 0;
        cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
      }
      if (funct3 == 0b0100000) { // sub
        cpu->control.alu_src    = 0;
        cpu->control.mem_to_reg = 0;
        cpu->control.reg_write  = 1;
        cpu->control.mem_read   = 0;
        cpu->control.mem_write  = 0;
        cpu->control.branch     = 0;
        cpu->control.branch_neq = 0;
        cpu->control.alu_op[0]  = 1;
        cpu->control.alu_op[1]  = 0;
        cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
      }
    }
  }

  // I-type
  if (opcode == 0b0000011) {
    if (funct3 == 0b010) { // lw
      cpu->control.alu_src    = 1;
      cpu->control.mem_to_reg = 1;
      cpu->control.reg_write  = 1;
      cpu->control.mem_read   = 1;
      cpu->control.mem_write  = 0;
      cpu->control.branch     = 0;
      cpu->control.branch_neq = 0;
      cpu->control.alu_op[0]  = 0;
      cpu->control.alu_op[1]  = 0;
      cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
    } 
  } else if (opcode == 0b0010011) {
    if (funct3 == 0b000) { // addi
      cpu->control.alu_src    = 1;
      cpu->control.mem_to_reg = 0;
      cpu->control.reg_write  = 1;
      cpu->control.mem_read   = 0;
      cpu->control.mem_write  = 0;
      cpu->control.branch     = 0;
      cpu->control.branch_neq = 0;
      cpu->control.alu_op[0]  = 0;
      cpu->control.alu_op[1]  = 0;
      cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
    }
  }

  // S-type
  if (opcode == 0b0100011) {
    if (funct3 == 0b010) { // sw
      cpu->control.alu_src    = 1;
    //cpu->control.mem_to_reg = X;
      cpu->control.reg_write  = 0;
      cpu->control.mem_read   = 0;
      cpu->control.mem_write  = 1;
      cpu->control.branch     = 0;
      cpu->control.branch_neq = 0;
      cpu->control.alu_op[0]  = 0;
      cpu->control.alu_op[1]  = 0;
      cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
    }
  }

  // SB-type
  if (opcode == 0b1100011) {
    if (funct3 == 0b000) { // beq
      cpu->control.alu_src    = 0;
    //cpu->control.mem_to_reg = X;
      cpu->control.reg_write  = 0;
      cpu->control.mem_read   = 0;
      cpu->control.mem_write  = 0;
      cpu->control.branch     = 1;
      cpu->control.branch_neq = 0;
      cpu->control.alu_op[0]  = 1;
      cpu->control.alu_op[1]  = 0;
      cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
    } else if (funct3 == 0b001) { // bne
      cpu->control.alu_src    = 0;
    //cpu->control.mem_to_reg = X;
      cpu->control.reg_write  = 0;
      cpu->control.mem_read   = 0;
      cpu->control.mem_write  = 0;
      cpu->control.branch     = 1;
      cpu->control.branch_neq = 1;
      cpu->control.alu_op[0]  = 1;
      cpu->control.alu_op[1]  = 0;
      cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
    }
  }

  cpu->alu.a = cpu->rf.read_data_a;
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
