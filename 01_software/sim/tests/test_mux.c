#include <assert.h>
#include "multiplexers.h"
#include "bus.h"

void test_mux() {
  assert(mux(0,0,0) == 0);
  assert(mux(0,0,1) == 0);
  assert(mux(0,1,0) == 0);
  assert(mux(0,1,1) == 1);
  assert(mux(1,0,0) == 1);
  assert(mux(1,0,1) == 0);
  assert(mux(1,1,0) == 1);
  assert(mux(1,1,1) == 1);
}

void test_mux64() {
  bus64 a, b, ra, rb;
  a = bus64_zero();
  b = bus64_ones();
  ra = mux64(a, b, 0);
  rb = mux64(a, b, 1);

  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(ra, i) == bus64_getbit(a, i));
  }

  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(rb, i) == bus64_getbit(b, i));
  }
}

int main() {
  test_mux();
  test_mux64();
}
