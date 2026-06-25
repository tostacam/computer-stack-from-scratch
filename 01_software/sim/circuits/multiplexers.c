#include "multiplexers.h"

bit mux(bit a, bit b, bit sel) {
  bit mux1 = AND(a, NOT(sel));
  bit mux2 = AND(b, sel);
  return OR(mux1, mux2);  
}

bus64 mux64(bus64 a, bus64 b, bit sel) {
  bus64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&r, i, mux(a.data[i], b.data[i], sel));
  }
  return r;
}
