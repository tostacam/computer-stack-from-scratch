#include "rom.h"

void ROM_init(ROM *rom, uint8_t *data, uint64_t size) {
  rom->size = size;
  for (uint64_t i = 0; i < size; ++i) {
    rom->bytes[i] = *data;
    ++data;
  }
}

uint8_t ROM_read(ROM *rom, uint64_t address) {
  return rom->bytes[address];
}
