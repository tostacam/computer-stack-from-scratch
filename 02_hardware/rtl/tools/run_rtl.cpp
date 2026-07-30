#include <verilated.h>
#include "Vcpu.h"
#include <iostream>

void tick(Vcpu &cpu);
void reset(Vcpu &cpu);

int main(int argc, char *argv[]) {

  Vcpu cpu;
  reset(cpu);

  std::cout << "PC: " << cpu.debug_pc
    << ", INST: " << std::hex << cpu.debug_instruction
    << std::dec << "\n";

  for (int i = 0; i < 3; i++) {
    tick(cpu);

    std::cout << "PC: " << cpu.debug_pc
      << ", INST: " << std::hex << cpu.debug_instruction
      << std::dec << "\n";
  }
}

void tick(Vcpu &cpu) {
  cpu.clk = 0;
  cpu.eval();

  cpu.clk = 1;
  cpu.eval();
}

void reset(Vcpu &cpu) {
  cpu.reset = 1;
  tick(cpu);
  tick(cpu);
  cpu.reset = 0;
}
