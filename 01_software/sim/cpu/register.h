#ifndef REGISTER_H
#define REGISTER_H

#include "bit.h"
#include "bus.h"
#include "flip-flop.h"

typedef struct {
  d_flip_flop ff[64];
  bit         enable;
} register64;

register64 register64_init();
void register64_input(register64 *r, bus64 input);
void register64_enable(register64 *r);
void register64_disable(register64 *r);
void register64_trigger(register64 *r);
bus64 register64_output(register64 *r);

#endif
