#ifndef RAM_H
#define RAM_H

#include "decoder_encoder.h"
#include "register.h"
#include "bus.h"
#include "bit.h"
#define RAM64_WORDS 64

// RAM64 = 64reg (1word/reg) = 64 words
typedef struct {
  register64  register_data[RAM64_WORDS];
  bus64       address;
  bus64       write_data;
  bit         write_enable;
  bus64       read_data;
  bit         read_enable;
} RAM64;

void RAM64_init(RAM64 *ram);
void RAM64_eval(RAM64 *ram);
void RAM64_tick(RAM64 *ram);

#endif
