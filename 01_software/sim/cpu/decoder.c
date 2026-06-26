#include "decoder.h"

unsigned int decode_amount(bus64 a) {
  unsigned int r = 0;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if (bus64_getbit(a, i)) {
      r |= (1U << i);
    }
  } 
  return r;
}
