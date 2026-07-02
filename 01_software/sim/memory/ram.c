#include "ram.h"

void RAM_init(RAM *ram) {
  for (int i = 0; i < RAM_SIZE; ++i) {
    ram->bytes[i] = 0;
  }
  ram->address      = 0;
  ram->write_data   = 0;
  ram->write_enable = 0;
  ram->read_data    = 0;
  ram->read_enable  = 0;
}

void RAM_read(RAM *ram, enum MEM_SIZE size) {
  if (ram->read_enable == 1) {
    switch(size) {
      case MEM_BYTE_SIZE:
        ram->read_data = ram->bytes[ram->address];
        break;
      case MEM_HALF_SIZE:
        ram->read_data = ram->bytes[ram->address] \
                       | (ram->bytes[ram->address + 1] << 8);
        break;
      case MEM_WORD_SIZE:
        ram->read_data = ram->bytes[ram->address] \
                       | (ram->bytes[ram->address + 1] << 8) \
                       | (ram->bytes[ram->address + 2] << 16) \
                       | (ram->bytes[ram->address + 3] << 24);
        break;
      case MEM_DWRD_SIZE:
        ram->read_data = (uint64_t)ram->bytes[ram->address] \
                       | ((uint64_t)ram->bytes[ram->address + 1] << 8) \
                       | ((uint64_t)ram->bytes[ram->address + 2] << 16) \
                       | ((uint64_t)ram->bytes[ram->address + 3] << 24) \
                       | ((uint64_t)ram->bytes[ram->address + 4] << 32) \
                       | ((uint64_t)ram->bytes[ram->address + 5] << 40) \
                       | ((uint64_t)ram->bytes[ram->address + 6] << 48) \
                       | ((uint64_t)ram->bytes[ram->address + 7] << 56);
        break;
    }
  }
  ram->read_enable = 0;
}

#define BYTE0_MASK 0x00000000000000FFULL
#define BYTE1_MASK 0x000000000000FF00ULL
#define BYTE2_MASK 0x0000000000FF0000ULL
#define BYTE3_MASK 0x00000000FF000000ULL
#define BYTE4_MASK 0x000000FF00000000ULL
#define BYTE5_MASK 0x0000FF0000000000ULL
#define BYTE6_MASK 0x00FF000000000000ULL
#define BYTE7_MASK 0xFF00000000000000ULL

void RAM_write(RAM *ram, enum MEM_SIZE size) {
  if (ram->write_enable == 1) {
    switch(size) {
      case MEM_BYTE_SIZE:
        ram->bytes[ram->address]     = (ram->write_data & BYTE0_MASK);
        break;
      case MEM_HALF_SIZE:
        ram->bytes[ram->address]     = (ram->write_data & BYTE0_MASK);
        ram->bytes[ram->address + 1] = (ram->write_data & BYTE1_MASK) >> 8;
        break;
      case MEM_WORD_SIZE:
        ram->bytes[ram->address]     = (ram->write_data & BYTE0_MASK);
        ram->bytes[ram->address + 1] = (ram->write_data & BYTE1_MASK) >> 8;
        ram->bytes[ram->address + 2] = (ram->write_data & BYTE2_MASK) >> 16;
        ram->bytes[ram->address + 3] = (ram->write_data & BYTE3_MASK) >> 24;
        break;
      case MEM_DWRD_SIZE:
        ram->bytes[ram->address]     = (ram->write_data & BYTE0_MASK);
        ram->bytes[ram->address + 1] = (ram->write_data & BYTE1_MASK) >> 8;
        ram->bytes[ram->address + 2] = (ram->write_data & BYTE2_MASK) >> 16;
        ram->bytes[ram->address + 3] = (ram->write_data & BYTE3_MASK) >> 24;
        ram->bytes[ram->address + 4] = (ram->write_data & BYTE4_MASK) >> 32;
        ram->bytes[ram->address + 5] = (ram->write_data & BYTE5_MASK) >> 40;
        ram->bytes[ram->address + 6] = (ram->write_data & BYTE6_MASK) >> 48;
        ram->bytes[ram->address + 7] = (ram->write_data & BYTE7_MASK) >> 56;
        break;
    }
  }
  ram->write_enable = 0;
}
