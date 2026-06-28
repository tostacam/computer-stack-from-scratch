#ifndef ROM_H
#define ROM_H

#include "bus.h"
#include "decoder_encoder.h"
#include <stdint.h>
#define ROM64_WORDS 64

typedef struct {
  bus64 words[ROM64_WORDS];
} ROM64;

void ROM64_init(ROM64 *rom, uint64_t *data, uint64_t size);
bus64 ROM64_read(ROM64 *rom, bus64 address);

#endif
