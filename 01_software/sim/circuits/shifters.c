#include "shifters.h"

bus64 shift_left(bus64 a, unsigned int n) {
  for (unsigned int j = 0 ; j < n; ++j) {
    for (int i = BUS64_WIDTH - 1; i > 0; --i) {
      bus64_setbit(&a, i, bus64_getbit(a, i - 1));
    }
    bus64_setbit(&a, 0, 0);
  }
  return a;
}

bus64 shift_right(bus64 a, unsigned int n) {
  for (unsigned int j = 0; j < n; ++j) {
    for (int i = 0; i < BUS64_WIDTH - 1; ++i) {
      bus64_setbit(&a, i, bus64_getbit(a, i + 1));
    }
    bus64_setbit(&a, BUS64_WIDTH - 1, 0);
  }
  return a;
}
