#ifndef ROM_H
#define ROM_H

#include <stdint.h>
#include "bus.h"
#include "decoder_encoder.h"
#define ROM_SIZE 64

typedef struct {
  uint8_t   bytes[ROM_SIZE];
  uint64_t  size;
} ROM;

void ROM_init(ROM *rom, uint8_t *data, uint64_t size);
uint8_t ROM_read(ROM *rom, uint64_t address);

#endif
