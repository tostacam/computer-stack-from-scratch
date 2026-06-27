#ifndef COUNTER_H
#define COUNTER_H

#include "register.h"
#include "adders.h"

typedef struct {
  bus64       count_in;
  bit         clear;
  bit         load;
  register64  output_reg;
} counter64;

void counter_init(counter64 *c);
void counter_tick(counter64 *c);

#endif
