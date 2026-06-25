#include <assert.h>
#include "shifters.h"

void test_shift_left() {
  bus64 r = bus64_ones();
  
  r = shift_left(r, 1);
  assert(bus64_getbit(r, 0) == 0);
  for (int i = 1; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = shift_left(r, BUS64_WIDTH - 1);
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
}

void test_shift_right() {
  bus64 r = bus64_ones();
  
  r = shift_right(r, 1);
  assert(bus64_getbit(r, BUS64_WIDTH - 1) == 0);
  for (int i = 0; i < BUS64_WIDTH - 1; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = shift_right(r, BUS64_WIDTH - 1);
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
}

int main() {
  test_shift_left();
  test_shift_right();
}
