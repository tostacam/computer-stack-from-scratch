#include "flip-flop.h"

d_flip_flop flip_flop_init() {
  d_flip_flop ff;
  ff.D = 0;
  ff.Q = 0;
  return ff;
}

void flip_flop_input(d_flip_flop *ff, bit input) {
  ff->D = input;
}

void flip_flop_trigger(d_flip_flop *ff) {
  ff->Q = ff->D;
}

bit flip_flop_output(d_flip_flop *ff) {
  return ff->Q;
}
