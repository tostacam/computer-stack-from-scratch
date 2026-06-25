#include <assert.h>
#include "adders.h"
#include "bus.h"

void test_halfadder() {
  HalfAdder h;
 
  h = half_adder(0, 0);
  assert(h.sum == 0 && h.carry_out == 0);
  h = half_adder(0, 1);
  assert(h.sum == 1 && h.carry_out == 0);
  h = half_adder(1, 0);
  assert(h.sum == 1 && h.carry_out == 0);
  h = half_adder(1, 1);
  assert(h.sum == 0 && h.carry_out == 1);  
}

void test_fulladder() {
  FullAdder f;

  f = full_adder(0, 0, 0);
  assert(f.sum == 0 && f.carry_out == 0);
  f = full_adder(0, 1, 0);
  assert(f.sum == 1 && f.carry_out == 0);
  f = full_adder(1, 0, 0);
  assert(f.sum == 1 && f.carry_out == 0);
  f = full_adder(1, 1, 0);
  assert(f.sum == 0 && f.carry_out == 1);
  f = full_adder(0, 0, 1);
  assert(f.sum == 1 && f.carry_out == 0);
  f = full_adder(0, 1, 1);
  assert(f.sum == 0 && f.carry_out == 1);
  f = full_adder(1, 0, 1);
  assert(f.sum == 0 && f.carry_out == 1);
  f = full_adder(1, 1, 1);
  assert(f.sum == 1 && f.carry_out == 1);
}

void test_add64() {
  bus64 a, b;
  add64_result r;
  
  a = bus64_zero();
  b = bus64_zero();
  r = add64(a, b, 0);
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r.bus64_sum, i) == 0);    
  }
  assert(r.carry_out == 0);

  a = bus64_ones();
  b = bus64_ones();
  r = add64(a, b, 1);
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r.bus64_sum, i) == 1);    
  }
  assert(r.carry_out == 1);
}

int main() {
  test_halfadder();
  test_fulladder();
  test_add64();
}
