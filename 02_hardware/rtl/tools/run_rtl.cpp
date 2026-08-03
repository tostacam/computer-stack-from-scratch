#include <verilated.h>
#include "Vcpu.h"
#include <iostream>

void tick(Vcpu *cpu);
void reset(Vcpu *cpu);
void CPU_run(Vcpu *cpu);
void output_results(Vcpu *cpu, const char *filename);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Need input/output file: <program.cpp> <input.hex> <output.json>");
    return 1;
  }

  // CPU init
  Verilated::commandArgs(argc, argv);
  Vcpu cpu;
  reset(&cpu);

  // CPU run
  CPU_run(&cpu);

  // Output
  output_results(&cpu, argv[2]);
}

void tick(Vcpu *cpu) {
  cpu->clk = 0;
  cpu->eval();

  cpu->clk = 1;
  cpu->eval();
}

void reset(Vcpu *cpu) {
  cpu->reset = 1;
  tick(cpu);
  tick(cpu);
  cpu->reset = 0;
}

void CPU_run(Vcpu *cpu) {
  std::cout << "PC: " << cpu->debug_pc
    << ", INST: " << std::hex << cpu->debug_instruction
    << std::dec << "\n";

  for (int i = 0; i < 3; i++) {
    tick(cpu);

    std::cout << "PC: " << cpu->debug_pc
      << ", INST: " << std::hex << cpu->debug_instruction
      << std::dec << "\n";
  } 
}

int ram_word(Vcpu *cpu, int i) {
  int word = 0;

  word |= cpu->debug_ram[i];
  word |= cpu->debug_ram[i + 1] << 8;
  word |= cpu->debug_ram[i + 2] << 16;
  word |= cpu->debug_ram[i + 3] << 24;

  return word;
}

void output_results(Vcpu *cpu, const char *filename) {
  FILE *fp = fopen(filename, "w");

  fprintf(fp, "{\n");
  fprintf(fp, "  \"pc\": %llu,\n", cpu->debug_pc);
  fprintf(fp, "  \"registers\": {\n");
  for (int i = 0; i < 32; ++i) {
    fprintf(fp, "    \"x%d\": %llu%s\n", i, cpu->debug_rf[i], (i == 31) ? "" : ",");
  }
  fprintf(fp, "  },\n");
  fprintf(fp, "  \"memory\": {\n");
  fprintf(fp, "    \"0x0000\": %d,\n", ram_word(cpu, 0));
  fprintf(fp, "    \"0x0004\": %d,\n", ram_word(cpu, 4));
  fprintf(fp, "    \"0x0008\": %d,\n", ram_word(cpu, 8));
  fprintf(fp, "    \"0x000C\": %d\n", ram_word(cpu, 12));
  fprintf(fp, "  }\n");
  fprintf(fp, "}\n");
  fclose(fp);
}
