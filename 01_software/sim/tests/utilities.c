#include "utilities.h"

bit is_equal_bus_uint(bus64 a, uint64_t n) {
  uint64_t bus_value = decode_amount(a);
  if (bus_value == n)
    return 1;
  else
    return 0;
}
