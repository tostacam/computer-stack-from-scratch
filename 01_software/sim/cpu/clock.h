#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

typedef struct {
  uint64_t cycles;
} clock;

void clock_init(clock *c);

void clock_tick(clock *c);

#endif
