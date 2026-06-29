#include "cpu.h"

void CPU_init(CPU *cpu, ROM64* rom) {
  program_counter_init(&cpu->core.pc);
  register_file_init(&cpu->core.rf);
  cpu->rom = rom;
  RAM64_init(&cpu->ram);
}

static void fetch(CPU *cpu);
static void decode(CPU *cpu);
static void execute(CPU *cpu);
static void write_back(CPU *cpu);
static void update_pc(CPU *cpu);

void CPU_cycle(CPU *cpu) {
  fetch(cpu);
  decode(cpu);
  execute(cpu);
  write_back(cpu);
  update_pc(cpu);
}
