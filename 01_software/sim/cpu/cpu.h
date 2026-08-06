#ifndef CPU_CORE_H
#define CPU_CORE_H

#include <assert.h>
#include <stdio.h>
#include "register_file.h"
#include "decoder_encoder.h"
#include "program_counter.h"
#include "alu.h"
#include "ram.h"
#include "rom.h"

enum control_opcodes {
  OPCODE_RTYPE  = 0b0110011,
  OPCODE_ITYPE  = 0b0010011,
  OPCODE_LOAD   = 0b0000011,
  OPCODE_STORE  = 0b0100011,
  OPCODE_BRANCH = 0b1100011,
  OPCODE_LUI    = 0b0110111,
  OPCODE_AUIPC  = 0b0010111,
  OPCODE_JAL    = 0b1101111,
  OPCODE_JALR   = 0b1100111,
  OPCODE_SYSTEM = 0b1110011
};

enum branch_codes {
  FUNCT3_BEQ  = 0b000,
  FUNCT3_BNE  = 0b001,
  FUNCT3_BLT  = 0b100,
  FUNCT3_BGE  = 0b101,
  FUNCT3_BLTU = 0b110,
  FUNCT3_BGEU = 0b111
};

enum wb_src {
  WB_ALU,
  WB_MEM,
  WB_PC4
};

enum pc_src {
  PC_NEXT,
  PC_BRANCH,
  PC_JAL,
  PC_JALR
};

typedef struct {
  bit         alu_src_a;
  bit         alu_src_b;
  enum wb_src wb_src;
  bit         reg_write;
  bit         mem_read;
  bit         mem_write;
  enum pc_src pc_src;
  uint8_t     alu_op;
} control_signals;

typedef struct {
  ALU             alu;
  program_counter pc;
  register_file   rf;
  ROM             rom;
  RAM             ram;

  bus64           instruction; 
  bus64           immediate;
  bus64           jump_address;
  control_signals control;
} CPU;

void CPU_init(CPU *cpu, ROM *rom);
void CPU_cycle(CPU *cpu);

#endif
