#include <verilated.h>
#include "Vsoc.h"
#include <iostream>

#define MAX_CYCLES  100
#define CPU_RUNNING 0

void tick(Vsoc *soc);
void reset(Vsoc *soc); 
void SOC_run(Vsoc *soc);
void output_results(Vsoc *soc, const char *filename);

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Need input/output file: <program.cpp> +ROM=<input.hex> +CYCLES=n <output.json>\n");
    return 1;
  }

  // CPU init
  Verilated::commandArgs(argc, argv);
  Vsoc soc;
  reset(&soc);

  // CPU run
  SOC_run(&soc);

  // Output
  output_results(&soc, argv[3]);
}

void tick(Vsoc *soc) {
  soc->clk = 0;
  soc->eval();

  soc->clk = 1;
  soc->eval();
}

void reset(Vsoc *soc) {
  soc->reset = 1;
  tick(soc);
  tick(soc);
  soc->reset = 0;
}

void SOC_run(Vsoc *soc) {
  int cycles = 0;

  while (soc->state == CPU_RUNNING && cycles < MAX_CYCLES) {
    tick(soc);
    ++cycles;
  }

  if (cycles == MAX_CYCLES) {
    printf("RTL test timed out\n");
  }
}

int ram_word(Vsoc *soc, int i) {
  int word = 0;

  word |= soc->debug_ram[i];
  word |= soc->debug_ram[i + 1] << 8;
  word |= soc->debug_ram[i + 2] << 16;
  word |= soc->debug_ram[i + 3] << 24;

  return word;
}

void output_results(Vsoc *soc, const char *filename) {
  FILE *fp = fopen(filename, "w");

  fprintf(fp, "{\n");
  fprintf(fp, "  \"pc\": %llu,\n", soc->debug_pc);
  fprintf(fp, "  \"registers\": {\n");
  for (int i = 0; i < 32; ++i) {
    fprintf(fp, "    \"x%d\": %llu%s\n", i, soc->debug_rf[31-i], (i == 31) ? "" : ",");
  }
  fprintf(fp, "  },\n");
  fprintf(fp, "  \"memory\": {\n");
  for (int i = 0; i < 16; ++i) {
    fprintf(fp, "    \"0x000%x\": %d%s\n", i, soc->debug_ram[i], (i == 15) ? "" : ",");
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
