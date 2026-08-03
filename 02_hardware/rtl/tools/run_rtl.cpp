#include <verilated.h>
#include "Vcpu.h"
#include <iostream>

void tick(Vcpu *cpu);
void reset(Vcpu *cpu);
void CPU_run(Vcpu *cpu);
void output_results(Vcpu *cpu, const char* filename);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Need input/output file: <program.cpp> <input.hex> <output.hex>");
    return 1;
  }

  // CPU init
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

void output_results(CPU *cpu, const char* filename) {
  FILE *fp = fopen(filename, "w");

  fprintf(fp, "{\n");
  fprintf(fp, "  \"pc\": %llu,\n", cpu->debug_pc);
  fprintf(fp, "  \"registers\": {\n");
  for (int i = 0; i < 32; ++i) {
    fprintf(fp, "    \"x%d\": %llu%s\n", i, cpu->debug_rf.register_data[i]), (i == 31) ? "" : ",";
  }
  fprintf(fp, "  },\n");
  fprintf(fp, "  \"memory\": {\n");
  fprintf(fp, "    \"0x0000\": %d,\n", ram_data(cpu, 0));
  fprintf(fp, "    \"0x0004\": %d,\n", ram_data(cpu, 4));
  fprintf(fp, "    \"0x0008\": %d,\n", ram_data(cpu, 8));
  fprintf(fp, "    \"0x000C\": %d\n", ram_data(cpu, 12));
  fprintf(fp, "  }\n");
  fprintf(fp, "}\n");

  fclose(fp);
}
