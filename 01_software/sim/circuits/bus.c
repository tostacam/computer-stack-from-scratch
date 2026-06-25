#include "bus.h"

void bus64_setbit(bus64 *b, int i, bit v) {
  b->data[i] = v;
}

bit bus64_getbit(bus64 b, int i) {
  return b.data[i];
}

bus64 bus64_zero() {
  bus64 b;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&b, i, 0);
  }
  return b;
}

bus64 bus64_ones() {
  bus64 b;
  for (int i =0 ; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&b, i, 1);
  }
  return b;
}
