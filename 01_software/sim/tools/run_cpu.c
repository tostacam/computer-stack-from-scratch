#ifndef RUN_CPU_H
#define RUN_CPU_H

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

void load_program(ROM *rom, const char *filename);
void CPU_run(CPU *cpu);
void output_results(CPU *cpu, const char *filename);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Need input/output file: <program.c> <input.hex> <output.json>\n");
    return 1;
  }

  // ROM load
  ROM rom;
  load_program(&rom, argv[1]);

  // CPU run
  CPU cpu;
  CPU_init(&cpu, &rom);
  CPU_run(&cpu);

  // Output
  output_results(&cpu, argv[2]);
}

void load_program(ROM *rom, const char *filename) {
  FILE *fp = fopen(filename, "r");
  assert(fp);

  uint32_t instruction, i = 0;

  while (fscanf(fp, "%x", &instruction) == 1) {
    rom->bytes[i++] = instruction & 0xFF;
    rom->bytes[i++] = (instruction >> 8) & 0xFF;
    rom->bytes[i++] = (instruction >> 16) & 0xFF;
    rom->bytes[i++] = (instruction >> 24) & 0xFF;
  }
  rom->size = i / 4;

  fclose(fp);
}

void CPU_run(CPU *cpu) {
  while (decode_amount(register64_output(&cpu->pc.output_reg)) < cpu->rom.size * 4) {
    CPU_cycle(cpu);
  }
}

int ram_data(CPU *cpu, int address) {
  return cpu->ram.bytes[address] | (cpu->ram.bytes[address] << 8) | (cpu->ram.bytes[address] << 16) | (cpu->ram.bytes[address] << 24);
}

void output_results(CPU *cpu, const char* filename) {
  FILE *fp = fopen(filename, "w");

  fprintf(fp, "{\n");
  fprintf(fp, "  \"pc\": %llu,\n", decode_amount(register64_output(&cpu->pc.output_reg)));
  fprintf(fp, "  \"registers\": {\n");
  for (int i = 0; i < 32; ++i) {
    fprintf(fp, "    \"x%d\": %llu%s\n", i, decode_amount(register64_output(&cpu->rf.register_data[i])), (i == 31) ? "" : ",");
  }
  fprintf(fp, "  },\n");
  fprintf(fp, "  \"memory\": {\n");
  for (int i = 0; i < 16; ++i) {
    fprintf(fp, "    \"0x000%x\": %d%s\n", i, cpu->ram.bytes[i], (i == 15) ? "" : ",");
  }
  /*
  fprintf(fp, "    \"0x0000\": %d,\n", ram_data(cpu, 0));
  fprintf(fp, "    \"0x0004\": %d,\n", ram_data(cpu, 4));
  fprintf(fp, "    \"0x0008\": %d,\n", ram_data(cpu, 8));
  fprintf(fp, "    \"0x000C\": %d\n", ram_data(cpu, 12));
  */
  fprintf(fp, "  }\n");
  fprintf(fp, "}\n");

  fclose(fp);
}

#endif
