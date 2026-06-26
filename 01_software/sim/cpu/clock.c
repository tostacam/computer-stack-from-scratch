#include "clock.h"

void clock_init(clock *c) {
  c.cycle = 0;
}

void clock_tick(clock *c) {
  // actual HW implementation would be an oscillator
  // A = NOT(A)
  c.cycle++;
}
