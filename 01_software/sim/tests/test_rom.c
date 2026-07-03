#include <assert.h>
#include <stdint.h>
#include "rom.h"

void test_rom() {
  ROM rom;

  // creating rom
  uint8_t data[3];
  data[0] = 20;
  data[1] = 65;
  data[2] = 90;
  ROM_init(&rom, data, 3ULL);

  uint8_t res[3];
  res[0] = 20;
  res[1] = 65;
  res[2] = 90;
  for (uint64_t i = 0; i < rom.size; ++i) {
    assert(ROM_read(&rom, i) == res[i]);
  }
}

int main() {
  test_rom();
}
