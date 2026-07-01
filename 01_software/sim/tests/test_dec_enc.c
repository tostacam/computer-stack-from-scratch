#include <assert.h>
#include "decoder_encoder.h"


#include <stdio.h>

void test_decode_amount() {
  uint64_t init, final;
  bus64 bus_a;

  // 0 -> encode -> decode -> 0
  init = 0;
  bus_a = encode_amount(init);
  final = decode_amount(bus_a);
  assert(final == init);

  // UINT64_MAX -> encode -> decode -> UINT64_MAX
  init = UINT64_MAX;
  bus_a = encode_amount(init);
  final = decode_amount(bus_a);
  assert(final == init);

  // random small val -> encode -> decode -> random small val
  init = 42;
  bus_a = encode_amount(init);
  final = decode_amount(bus_a);
  assert(final == init);
}

void test_decode_nbits() {
  uint64_t init, final;
  bus64 bus_a;

  // 1111 1100 -> encode -> decode_nbits(2 to 7) -> 0011 1111
  init = 252ULL;
  bus_a = encode_amount(init);
  final = decode_nbits(bus_a, 2, 7);
  printf("value: %llu\n", final);
  assert(final == 63ULL);
}

int main() {
  test_decode_amount();
  test_decode_nbits();
}
