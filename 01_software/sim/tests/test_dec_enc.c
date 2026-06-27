#include <assert.h>
#include "decoder_encoder.h"

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

int main() {
  test_decode_amount();
}
