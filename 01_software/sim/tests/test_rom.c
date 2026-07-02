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

  assert(ROM_read(&rom, 0) == 20U);
  assert(ROM_read(&rom, 1) == 65U);
  assert(ROM_read(&rom, 2) == 90U);
}

int main() {
  test_rom();
}
