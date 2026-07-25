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

typedef struct {
  bit     alu_src;
  bit     mem_to_reg;
  bit     reg_write;
  bit     mem_read;
  bit     mem_write;
  bit     branch;
  bit     branch_neq;
  uint8_t alu_op;
} control_signals;

typedef struct {
  ALU             alu;
  program_counter pc;
  register_file   rf;
  ROM             rom;
  RAM             ram;

  bus64           instruction; 
  bus64           jump_address;
  control_signals control;
} CPU;

void CPU_init(CPU *cpu, ROM *rom);
void CPU_cycle(CPU *cpu);

#endif
