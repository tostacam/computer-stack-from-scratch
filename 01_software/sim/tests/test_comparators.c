#include <assert.h>
#include "comparators.h"

void test_is_zero64() {
  bus64 a = bus64_zero();
  assert(is_zero64(a) == 1);

  bus64 b = bus64_ones();
  assert(is_zero64(b) == 0);
}

void test_is_equal64() {
  bus64 a = bus64_ones();
  bus64 b = bus64_ones();
  bus64 c = bus64_zero();

  assert(is_equal64(a, b) == 1);
  assert(is_equal64(a, c) == 0);
}

int main() {
  test_is_zero64();
  test_is_equal64();
}
