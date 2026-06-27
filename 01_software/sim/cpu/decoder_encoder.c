#include "decoder_encoder.h"

uint64_t decode_amount(bus64 a) {
  uint64_t r = 0;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if (bus64_getbit(a, i)) {
      r |= (1ULL << i);
    }
  } 
  return r;
}

bus64 encode_amount(uint64_t n) {
  bus64 r = bus64_zero();
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    if ((n >> i) & 1ULL) {
      bus64_setbit(&r, i, 1);
    }
  }
  return r;
}
