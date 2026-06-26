#ifndef FLIP_FLOP_H
#define FLIP_FLOP_H

#include "bit.h"

typedef struct {
  bit D;
  bit Q;
} d_flip_flop;

d_flip_flop flip_flop_init();
void flip_flop_input(d_flip_flop *ff, bit input);
void flip_flop_trigger(d_flip_flop *ff);
bit flip_flop_output(d_flip_flop *ff);

#endif
