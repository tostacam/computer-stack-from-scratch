#include <verilated.h>
#include "Vsoc.h"
#include <iostream>
#include <vector>

#define MAX_CYCLES    100'000
#define CPU_RUNNING   0
#define SOC_MEM_SIZE  4
#define SOC_MEM_START 0

struct SOC_data {
  Vsoc system;
  std::vector<uint8_t> uart;
};

void tick(Vsoc *soc);
void reset(Vsoc *soc); 
void SOC_run(SOC_data *soc);
void output_results(Vsoc *soc, std::vector<uint8_t> *uart, const char *filename);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Need input/output file: <program.cpp> +ROM=<input.hex> <output.json>\n");
    return 1;
  }

  // SOC init
  Verilated::commandArgs(argc, argv);
  SOC_data soc;
  reset(&soc.system);

  // SOC run
  SOC_run(&soc);

  // Output
  output_results(&soc.system, &soc.uart, argv[2]);
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

void SOC_run(SOC_data *soc) {
  const int CLKS_PER_BIT = 100'000'000 / 115'200;

  int cycles = 0;
  uint8_t uart_data = 0;

  while (cycles < MAX_CYCLES) {

    // run until UART is set to low
    while (soc->system.uart_tx == 1 && cycles < MAX_CYCLES) {
      tick(&soc->system);
      ++cycles;
    }

    if (cycles == MAX_CYCLES) {
      return;
    }

    // middle of first data bit
    for (int i = 0; i < CLKS_PER_BIT + CLKS_PER_BIT/2; ++i) {
      tick(&soc->system);
      ++cycles;
    }

    uart_data = 0;

    // capturing 8 data bits
    for (int bit = 0; bit < 8; ++bit) {
      if (soc->system.uart_tx) {
        uart_data |= (1 << bit);
      }

      for (int i = 0; i < CLKS_PER_BIT; ++i) {
        tick(&soc->system);
        ++cycles;
      }
    }

    soc->uart.push_back(uart_data);
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

void output_results(Vsoc *soc, std::vector<uint8_t> *uart, const char *filename) {
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
  fprintf(fp, "  },\n");
  if (uart->empty()) {
    fprintf(fp, "  \"uart\": \"\"\n");
  }
  else {
    fprintf(fp, "  \"uart\": \"");
    for (auto c : *uart) {
      fprintf(fp, "%c", static_cast<int>(c));
    }
    fprintf(fp, "\"\n");
  }
  fprintf(fp, "}\n");
  fclose(fp);
}
 
