#include "rom.h"

void ROM64_init(ROM64 *rom, uint64_t *data, uint64_t size) {
  for (uint64_t i = 0; i < ROM64_WORDS; ++i) {
    if (i < size) {
      rom->words[i] = encode_amount(*data);
      ++data;
    } else {
      rom->words[i] = bus64_zero();
    }
  }
}

bus64 ROM64_read(ROM64 *rom, bus64 address) {
  return rom->words[decode_amount(address)];
}
