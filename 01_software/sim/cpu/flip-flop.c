#include "flip-flop.h"

void flip_flop_init(d_flip_flop *ff) {
  ff->D = 0;
  ff->Q = 0;
}

void flip_flop_trigger(d_flip_flop *ff) {
  ff->Q = ff->D;
}
