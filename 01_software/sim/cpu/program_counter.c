#include "program_counter.h"

void program_counter_init(program_counter *pc) {
  pc->count_inc   = encode_amount(INSTRUCTION_WIDTH);
  pc->jump_addr   = bus64_zero();
  pc->clear       = 0;
  pc->jump        = 0;
  pc->output_reg  = register64_init();
}

void program_counter_tick(program_counter *pc) {
  bus64 next_addr = register64_output(&pc->output_reg);

  next_addr = pc->clear ? bus64_zero() : add64_no_crry(next_addr, pc->count_inc);
  next_addr = pc->jump ? pc->jump_addr : next_addr; 
  register64_input(&pc->output_reg, next_addr);

  pc->output_reg.enable = 1;
  register64_trigger(&pc->output_reg);
  pc->output_reg.enable = 0;
}
