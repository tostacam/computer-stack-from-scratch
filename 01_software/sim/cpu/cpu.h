#ifndef CPU_CORE_H
#define CPU_CORE_H

#include "register_file.h"
#include "program_counter.h"
#include "alu.h"

typedef struct {
  program_counter pc;
  register_file   rf;
  ROM64           rom;
  RAM64           ram; 
} cpu_core;

typedef struct {
  // privaten implementation
  cpu_core core;

  // input
  bus64 input_mem;
  bus64 instruction;
  bit reset;

  // output
  bus64 output;
  bit write_mem;
  bus64 address_mem;
  bus64 program_count;
} CPU;

#endif

void CPU_init(CPU *cpu, ROM64 *rom);
void CPU_cycle(CPU *cpu);
