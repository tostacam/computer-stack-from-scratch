#include <assert.h>
#include "bus.h"

void test_bus64_zero() {
  bus64 b = bus64_zero();
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(b.data[i] == 0);
  }
}

void test_bus64_getbit() {
  bus64 b = bus64_zero();
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(b, i) == 0);
  }
}

void test_bus64_setbit() {
  bus64 b = bus64_zero();
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&b, i, 1);
    assert(bus64_getbit(b, i) == 1);
  }
}

int main() {
  test_bus64_zero();
  test_bus64_getbit();
  test_bus64_setbit();
}
