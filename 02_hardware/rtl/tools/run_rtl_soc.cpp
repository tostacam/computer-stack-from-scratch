#include <verilated.h>
#include "Vsoc.h"
#include <iostream>

#define MAX_CYCLES    10'000
#define CPU_RUNNING   0
#define SOC_MEM_SIZE  4
#define SOC_MEM_START 0

void tick(Vsoc *soc);
void reset(Vsoc *soc); 
void SOC_run(Vsoc *soc);
void output_results(Vsoc *soc, const char *filename);
uint8_t capture_uart(Vsoc *soc);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Need input/output file: <program.cpp> +ROM=<input.hex> <output.json>\n");
    return 1;
  }

  // SOC init
  Verilated::commandArgs(argc, argv);
  Vsoc soc;
  reset(&soc);

  // SOC run
  SOC_run(&soc);
  uint8_t uart_data = 0;//capture_uart(&soc);

  // UART
  if (uart_data != 0x41) {
    printf("  *UART test failed!\n");
  } else {
    printf("  *UART test passed!\n");
  }

  // Output
  output_results(&soc, argv[2]);
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
  for (int i = SOC_MEM_START; i < SOC_MEM_START + SOC_MEM_SIZE; ++i) {
    fprintf(fp, "    \"0x%05d\": %d%s\n", i, ram_word(soc, i), (i == SOC_MEM_START + SOC_MEM_SIZE - 1) ? "" : ",");
  }
  fprintf(fp, "  }\n");
  fprintf(fp, "}\n");
  fclose(fp);
}

uint8_t capture_uart(Vsoc* soc) {
  const int CLKS_PER_BIT = 100'000'000 / 115'200;
  
  // uart is idle
  while (soc->uart_tx == 1) {
    tick(soc);
  }

  for (int i = 0; i < CLKS_PER_BIT + CLKS_PER_BIT/2; ++i) {
    tick(soc);
  }

  uint8_t data = 0;

  // capturing 8 bits
  for (int bit = 0; bit < 8; ++ bit) {
    if (soc->uart_tx) {
      data |= (1 << bit);
    }

    for (int i = 0; i < CLKS_PER_BIT; ++i) {
      tick(soc);
    }
  }

  return data;
} 
