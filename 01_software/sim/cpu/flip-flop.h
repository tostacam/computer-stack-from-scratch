#ifndef FLIP_FLOP_H
#define FLIP_FLOP_H

#include "bit.h"

typedef struct {
  bit D;
  bit Q;
} d_flip_flop;

void flip_flop_init(d_flip_flop *ff);
void flip_flop_trigger(d_flip_flop *ff);

#endif
