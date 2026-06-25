#include "bus.h"

bus64 bus64_zero() {
  bus64 b;
  for (int i = 0; i < BUS64_WIDTH; i++) {
    b.data[i] = 0;    
  }
  return b;
}

bit bus64_getbit(bus64 b, int i) {
  return b.data[i];
}

void bus64_setbit(bus64 *b, int i, bit v) {
  b->data[i] = v;
}
