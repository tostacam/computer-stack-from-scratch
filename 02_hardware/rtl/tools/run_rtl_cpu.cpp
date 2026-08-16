#include <verilated.h>
#include "Vcpu_system.h"
#include <iostream>

#define MAX_CYCLES  100
#define CPU_RUNNING 0

void tick(Vcpu_system *cpu);
void reset(Vcpu_system *cpu); 
void CPU_run(Vcpu_system *cpu);
void output_results(Vcpu_system *cpu, const char *filename);

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Need input/output file: <program.cpp> +ROM=<input.hex> +CYCLES=n <output.json>\n");
    return 1;
  }

  // CPU init
  Verilated::commandArgs(argc, argv);
  Vcpu_system cpu;
  reset(&cpu);

  // CPU run
  CPU_run(&cpu);

  // Output
  output_results(&cpu, argv[3]);
}

void tick(Vcpu_system *cpu) {
  cpu->clk = 0;
  cpu->eval();

  cpu->clk = 1;
  cpu->eval();
}

void reset(Vcpu_system *cpu) {
  cpu->reset = 1;
  tick(cpu);
  tick(cpu);
  cpu->reset = 0;
}

void CPU_run(Vcpu_system *cpu) {
  int cycles = 0;

  while (cpu->state == CPU_RUNNING && cycles < MAX_CYCLES) {
    tick(cpu);
    ++cycles;
  }

  if (cycles == MAX_CYCLES) {
    printf("RTL test timed out\n");
  }
}

int ram_word(Vcpu_system *cpu, int i) {
  int word = 0;

  word |= cpu->debug_ram[i];
  word |= cpu->debug_ram[i + 1] << 8;
  word |= cpu->debug_ram[i + 2] << 16;
  word |= cpu->debug_ram[i + 3] << 24;

  return word;
}

void output_results(Vcpu_system *cpu, const char *filename) {
  FILE *fp = fopen(filename, "w");

  fprintf(fp, "{\n");
  fprintf(fp, "  \"pc\": %llu,\n", cpu->debug_pc);
  fprintf(fp, "  \"registers\": {\n");
  for (int i = 0; i < 32; ++i) {
    fprintf(fp, "    \"x%d\": %llu%s\n", i, cpu->debug_rf[31-i], (i == 31) ? "" : ",");
  }
  fprintf(fp, "  },\n");
  fprintf(fp, "  \"memory\": {\n");
  for (int i = 0; i < 16; ++i) {
    fprintf(fp, "    \"0x000%x\": %d%s\n", i, cpu->debug_ram[i], (i == 15) ? "" : ",");
  }
  /*
  fprintf(fp, "    \"0x0000\": %d,\n", ram_word(cpu, 0));
  fprintf(fp, "    \"0x0004\": %d,\n", ram_word(cpu, 4));
  fprintf(fp, "    \"0x0008\": %d,\n", ram_word(cpu, 8));
  fprintf(fp, "    \"0x000C\": %d\n", ram_word(cpu, 12));
  */
  fprintf(fp, "  }\n");
  fprintf(fp, "}\n");
  fclose(fp);
}
