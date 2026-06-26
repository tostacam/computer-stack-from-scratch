#include "bus.h"

bus64 encode_amount(unsigned int n) {
  bus64 r = bus64_zero();
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if ((n >> 1) & 1U) {
      bus64_setbit(&r, i, 1);
    }
  }
  return r;
}
