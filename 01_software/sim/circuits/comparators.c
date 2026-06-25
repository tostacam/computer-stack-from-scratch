#include "comparators.h"

bit is_zero64(bus64 a) {
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if (bus64_getbit(a, i) == 1) 
      return 0;
  }
  return 1;
}

bit is_equal64(bus64 a, bus64 b) {
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if (bus64_getbit(a, i) != bus64_getbit(b, i))
      return 0;
  }
  return 1;
}
