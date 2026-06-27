#include "counter.h"

void counter_init(counter64 *c) {
  c->count_in   = bus64_zero();
  c->load       = 0;
  c->clear      = 0;
  c->increment  = 0;
  c->output_reg = register64_init();
}

void counter_tick(counter64 *c) {
  if (c->clear == 1) {
    register64_input(&(c->output_reg), bus64_zero());
  } else if (c->load == 1) {
    register64_input(&(c->output_reg), c->count_in);
  } else if (c->increment == 1) {
    add64_result res = add64(register64_output(&(c->output_reg)), bus64_zero(), 1);
    register64_input(&(c->output_reg), res.bus64_sum);
  } else {
    register64_input(&(c->output_reg), register64_output(&(c->output_reg)));
  }
  c->output_reg.enable = 1;
  register64_trigger(&(c->output_reg));
  c->output_reg.enable = 0;
}
