#include "decoder.h"

uint64_t decode_amount(bus64 a) {
  uint64_t r = 0;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if (bus64_getbit(a, i)) {
      r |= (1ULL << i);
    }
  } 
  return r;
}
