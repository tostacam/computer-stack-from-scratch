#include <assert.h>
#include <stdint.h>
#include "utilities.h"
#include "rom.h"

void test_rom() {
  ROM64 rom;
  bus64 address;

  // creating rom
  uint64_t data[3];
  data[0] = 20;
  data[1] = 65;
  data[2] = 90;
  ROM64_init(&rom, data, 3ULL);

  address = encode_amount(0ULL);
  assert(is_equal_bus_uint(ROM64_read(&rom, address), 20ULL) == 1);
  address = encode_amount(1ULL);
  assert(is_equal_bus_uint(ROM64_read(&rom, address), 65ULL) == 1);
  address = encode_amount(2ULL);
  assert(is_equal_bus_uint(ROM64_read(&rom, address), 90ULL) == 1);
}

int main() {
  test_rom();
}
