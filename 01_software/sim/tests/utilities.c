#include "utilities.h"

bus64 encode_amount(uint64_t n) {
  bus64 r = bus64_zero();
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if ((n >> i) & 1ULL) {
      bus64_setbit(&r, i, 1);
    }
  }
  return r;
}
