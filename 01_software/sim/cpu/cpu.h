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

typedef struct {
  bit alu_src;
  bit mem_to_reg;
  bit reg_write;
  bit mem_read;
  bit mem_write;
  bit branch;
  bit branch_neq;
  bit alu_op[2];
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
