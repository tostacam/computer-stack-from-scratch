#include "register.h"

register64 register64_init() {
  register64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    r.ff[i] = flip_flop_init();
  }
  r.load = 0;
  return r;
}

void register64_input(register64 *r, bus64 input) {
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    flip_flop_input(&(r->ff[i]), bus64_getbit(input, i));
  }
}

void register64_enable(register64 *r) {
  r->load = 1;
}

void register64_disable(register64 *r) {
  r->load = 0;
}

void register64_trigger(register64 *r) {
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if (r->load == 1) {
      flip_flop_trigger(&(r->ff[i]));
    }
  }
}

bus64 register64_output(register64 *r) {
  bus64 output = bus64_zero();
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit( 
        &output,
        i,
        flip_flop_output(&(r->ff[i]))
        );
  }
  return output;
}
