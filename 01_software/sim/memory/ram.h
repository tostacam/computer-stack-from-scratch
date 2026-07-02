#ifndef RAM_H
#define RAM_H

#include <stdint.h>
#include "decoder_encoder.h"
#include "register.h"
#include "bus.h"
#include "bit.h"
#define RAM_SIZE 1024

enum MEM_SIZE {
  MEM_BYTE_SIZE = 1,
  MEM_HALF_SIZE = 2,
  MEM_WORD_SIZE = 4,
  MEM_DWRD_SIZE = 8
};

typedef struct {
  uint8_t   bytes[RAM_SIZE];
  uint64_t  address;
  uint64_t  write_data;
  uint64_t  read_data; 
  bit       write_enable;
  bit       read_enable;
} RAM;

void RAM_init(RAM *ram);
void RAM_read(RAM *ram, enum MEM_SIZE size);
void RAM_write(RAM *ram, enum MEM_SIZE size);

#endif
