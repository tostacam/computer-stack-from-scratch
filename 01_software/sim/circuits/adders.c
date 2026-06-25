#include "adders.h"

HalfAdder half_adder(bit a, bit b) {
  HalfAdder h;
  h.sum = XOR(a, b);
  h.carry_out = AND(a, b);
  return h;
}

FullAdder full_adder(bit a, bit b, bit cin) {
  HalfAdder h1 = half_adder(a, b);
  HalfAdder h2 = half_adder(cin, h1.sum);
 
  FullAdder f;
  f.sum = h2.sum;
  f.carry_out = OR(h1.carry_out, h2.carry_out);
  return f;
}

add64_result add64(bus64 a, bus64 b, bit cin) {
  add64_result r;
  bit next_carry = cin;
  FullAdder f;

  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bit a_i = bus64_getbit(a, i);
    bit b_i = bus64_getbit(b, i);
  
    f = full_adder(a_i, b_i, next_carry);
    bus64_setbit(&r.bus64_sum, i, f.sum);
    
    next_carry = f.carry_out;
  }
  r.carry_out = next_carry;

  return r;
}
