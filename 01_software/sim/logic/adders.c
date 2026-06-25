#include "adders.h"

HalfAdder half_adder(bit a, bit b) {
  HalfAdder h;
  h.sum = XOR(a, b);
  h.carry = AND(a, b);
  return h;
}

FullAdder full_adder(bit a, bit b, bit cin) {
  HalfAdder h1 = half_adder(a, b);
  HalfAdder h2 = half_adder(cin, h1.sum);
 
  FullAdder f;
  f.sum = h2.sum;
  f.carry = OR(h1.carry, h2.carry);
}
