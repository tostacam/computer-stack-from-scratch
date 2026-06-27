#include "program_counter.h"

void program_counter_init(program_counter *pc) {
  pc->jump_addr   = bus64_zero();
  pc->clear       = 0;
  pc->jump        = 0;
  pc->output_reg  = register64_init();
}

void program_counter_tick(program_counter *pc) {
  add64_result  res_step;
  bus64         output;
 
  res_step = add64(register64_output(&pc->output_reg), encode_amount(INSTRUCTION_WIDTH), 0);
  pc->count_in = res_step.bus64_sum;
  output = mux64(pc->count_in, bus64_zero(), pc->clear);
  output = mux64(output, pc->jump_addr, pc->jump);
  register64_input(&pc->output_reg, output);

  pc->output_reg.enable = 1;
  register64_trigger(&pc->output_reg);
  pc->output_reg.enable = 0;
}
