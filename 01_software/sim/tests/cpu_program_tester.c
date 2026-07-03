#include <assert.h>
#include <stdio.h>
#include "cpu.h"

void test_add_program() {
  /*
   * addi x1, x0, 5
   * addi x2, x0, 7
   * add x3, x1, x2
   *
   */

  // ROM Load
  ROM rom;
  uint32_t program_code[3];
  program_code[0] = 0x00500093;
  program_code[1] = 0x00700113;
  program_code[2] = 0x002081B3;
  ROM_init(&rom, (uint8_t *)program_code, 3 * sizeof(uint32_t));

  // CPU execute
  CPU cpu;
  CPU_init(&cpu, &rom);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);

  // Result
  printf("\nADD\n");
  for (int i = 1; i < 4; ++i) {
    cpu.rf.read_addr_a = encode_amount(i);
    register_file_eval(&cpu.rf);
    printf("reg x[%d]: %llu\n", i, decode_amount(cpu.rf.read_data_a));
  }
  printf("PC: %llu\n", decode_amount(register64_output(&cpu.pc.output_reg)));
}

void test_lwsw_program() {
  /*
   * addi x1, x0, 100
   * addi x2, x0, 42
   * sw   x2, 0(x1)
   * lw   x3, 0(x1)
   *
   */

  // ROM Load
  ROM rom;
  uint32_t program_code[4];
  program_code[0] = 0x06400093;
  program_code[1] = 0x02A00113;
  program_code[2] = 0x0020A023;
  program_code[3] = 0x0000A183;
  ROM_init(&rom, (uint8_t *)program_code, 4 * sizeof(uint32_t));

  // CPU execute
  CPU cpu;
  CPU_init(&cpu, &rom);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);

  // Result
  printf("\nLW/SW\n");
  for (int i = 1; i < 4; ++i) {
    cpu.rf.read_addr_a = encode_amount(i);
    register_file_eval(&cpu.rf);
    printf("reg x[%d]: %llu\n", i, decode_amount(cpu.rf.read_data_a));
  }

  printf("RAM[100]: ");
  for (int i = 0; i < 4; ++i) {
    printf("%u", cpu.ram.bytes[103 - i]);
  }
  printf("\n");
  printf("PC: %llu\n", decode_amount(register64_output(&cpu.pc.output_reg)));
}

void test_beq_program() {
  /*
   * addi x1, x0, 5
   * addi x2, x0, 5
   * beq  x1, x2, +8
   * addi x3, x0, 1
   * addi x4, x0, 2
   *
   */

  // ROM Load
  ROM rom;
  uint32_t program_code[5];
  program_code[0] = 0x00500093;
  program_code[1] = 0x00500113;
  program_code[2] = 0x00208463;
  program_code[3] = 0x00100193;
  program_code[4] = 0x00200213;
  ROM_init(&rom, (uint8_t *)program_code, 5 * sizeof(uint32_t));

  // CPU execute
  CPU cpu;
  CPU_init(&cpu, &rom);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);

  // Result
  printf("\nBEQ\n");
  for (int i = 1; i < 5; ++i) {
    cpu.rf.read_addr_a = encode_amount(i);
    register_file_eval(&cpu.rf);
    printf("reg x[%d]: %llu\n", i, decode_amount(cpu.rf.read_data_a));
  }
  printf("PC: %llu\n", decode_amount(register64_output(&cpu.pc.output_reg)));

}

void test_bne_program() {
  /*
   * addi x1, x0, 5
   * addi x2, x0, 7
   * bne  x1, x2, +8
   * addi x3, x0, 1
   * addi x4, x0, 2
   *
   */

  // ROM Load
  ROM rom;
  uint32_t program_code[5];
  program_code[0] = 0x00500093;
  program_code[1] = 0x00700113;
  program_code[2] = 0x00209463;
  program_code[3] = 0x00100193;
  program_code[4] = 0x00200213;
  ROM_init(&rom, (uint8_t *)program_code, 5 * sizeof(uint32_t));

  // CPU execute
  CPU cpu;
  CPU_init(&cpu, &rom);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);
  CPU_cycle(&cpu);

  // Result
  printf("\nBEQ\n");
  for (int i = 1; i < 5; ++i) {
    cpu.rf.read_addr_a = encode_amount(i);
    register_file_eval(&cpu.rf);
    printf("reg x[%d]: %llu\n", i, decode_amount(cpu.rf.read_data_a));
  }
  printf("PC: %llu\n", decode_amount(register64_output(&cpu.pc.output_reg)));

}

int main() {
  test_add_program();
  test_lwsw_program();
  test_beq_program();
  test_bne_program();
}
