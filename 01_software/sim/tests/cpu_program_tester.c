#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

typedef struct {
  uint64_t regs[32];
  uint64_t pc;
} CPU_expected;

void load_program(ROM *rom, const char *filename);
void load_expected(CPU_expected *expected, const char *filename);
void CPU_run(CPU *cpu);
void verify(CPU *cpu, CPU_expected *expected);

int main(){
  
  // Load progam into ROM
  ROM rom;
  load_program(&rom, "tests/programs/bne.txt");

  // Load ecpted results
  CPU_expected expected = {0};
  load_expected(&expected, "tests/programs/bne.exp");

  // Run CPU
  CPU cpu;
  CPU_init(&cpu, &rom);
  CPU_run(&cpu);

  // Compare results
  verify(&cpu, &expected);

  printf("TEST PASSED");
}

void load_program(ROM *rom, const char *filename) {
  FILE *fp = fopen(filename, "r");
  assert(fp);
  

  uint32_t instr;
  int i = 0;

  while (fscanf(fp, "%x", &instr) == 1) {
    rom->bytes[i++] = instr & 0xFF;
    rom->bytes[i++] = (instr >> 8) & 0xFF;
    rom->bytes[i++] = (instr >> 16) & 0xFF;
    rom->bytes[i++] = (instr >> 24) & 0xFF;
  }
  rom->size = i / 4;
  
  fclose(fp);
}

void load_expected(CPU_expected *expected, const char *filename) {
  FILE *fp = fopen(filename, "r");
  assert(fp);

  char name[32];
  uint32_t value;

  while (fscanf(fp, "%31[^=]=%u\n", name, &value) == 2) {
    if (name[0] == 'x')
      expected->regs[atoi(name + 1)] = value;
    else if (strcmp(name, "pc") == 0)
      expected->pc = value;
  }

  fclose(fp);
}

void CPU_run(CPU *cpu) {
  while (decode_amount(register64_output(&cpu->pc.output_reg)) < cpu->rom.size * 4) {
    CPU_cycle(cpu);
  }
}

void verify(CPU *cpu, CPU_expected *expected) {
  for (int i = 0; i < 32; i++) {
    assert(decode_amount(register64_output(&cpu->rf.register_data[i])) == expected->regs[i]);
  }
  assert(decode_amount(register64_output(&cpu->pc.output_reg)) == expected->pc);
}
