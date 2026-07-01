#include "cpu.h"

void CPU_init(CPU *cpu, ROM64 *rom) {
  program_counter_init(&cpu->pc);
  cpu->rom = *rom;
  register_file_init(&cpu->rf);
  alu_init(&cpu->alu);
  RAM64_init(&cpu->ram);

  cpu->instruction        = bus64_zero();
  cpu->control.alu_src    = 0;
  cpu->control.mem_to_reg = 0;
  cpu->control.reg_write  = 0;
  cpu->control.mem_read   = 0;
  cpu->control.mem_write  = 0;
  cpu->control.branch     = 0;
  cpu->control.alu_op[0]  = 0;
  cpu->control.alu_op[1]  = 0;
}

static void fetch(CPU *cpu) {
  cpu->instruction = cpu->rom.words[decode_amount(register64_output(&cpu->pc.output_reg))];
}

static enum alu_op alu_control(bit alu_op[2], uint8_t funct3, uint8_t funct7) {
  // 00 -> always ADD
  if (alu_op[0] == 0 && alu_op[0] == 0)
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

  return ALU_OP_NOP;
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
    cpu->control.alu_src    = 0;
    cpu->control.mem_to_reg = 0;
    cpu->control.reg_write  = 1;
    cpu->control.mem_read   = 0;
    cpu->control.mem_write  = 0;
    cpu->control.branch     = 0;
    cpu->control.alu_op[0]  = 1;
    cpu->control.alu_op[1]  = 0;
    cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
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
      cpu->control.alu_op[0]  = 0;
      cpu->control.alu_op[1]  = 0;
      cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
    }
  }

  // SB-type
  if (opcode == 0b1100111) {
    if (funct3 == 0b000) { // beq
      cpu->control.alu_src    = 0;
    //cpu->control.mem_to_reg = X;
      cpu->control.reg_write  = 0;
      cpu->control.mem_read   = 0;
      cpu->control.mem_write  = 0;
      cpu->control.branch     = 1;
      cpu->control.alu_op[0]  = 0;
      cpu->control.alu_op[1]  = 1;
      cpu->alu.opcode         = alu_control(cpu->control.alu_op, funct3, funct7);
    } else if (funct3 == 0b001) { // bne
      cpu->control.alu_src    = 0;
    //cpu->control.mem_to_reg = X;
      cpu->control.reg_write  = 0;
      cpu->control.mem_read   = 0;
      cpu->control.mem_write  = 0;
      cpu->control.branch     = 1;
      cpu->control.alu_op[0]  = 0;
      cpu->control.alu_op[1]  = 1;
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
      case 0b1100111: // SB-type
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
}

static void execute(CPU *cpu) {
  alu_eval(&cpu->alu);


}

//static void memory_access(CPU *cpu);
//static void write_back(CPU *cpu);

void CPU_cycle(CPU *cpu) {
  fetch(cpu);
  decode(cpu);
  execute(cpu);
  /*
  memory_access(cpu);
  write_back(cpu);
  */
  register_file_tick(&cpu->rf);
}
