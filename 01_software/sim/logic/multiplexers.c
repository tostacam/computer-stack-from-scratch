#include "multiplexers.h"

bit mux(bit a, bit b, bit sel) {
  bit mux1 = AND(a, sel);
  bit mux2 = AND(b, NOT(sel));
  return OR(mux1, mux2);  
}

bus64 mux64(bus64 a, bus64 b, bit sel) {
  bus64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&r, i, mux(a[i], b[i], sel));
  }
  return r;
}
