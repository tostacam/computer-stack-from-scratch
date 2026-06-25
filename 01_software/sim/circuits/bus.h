#ifndef BUS_H
#define BUS_H

#include "bit.h"

#define BUS64_WIDTH 64

typedef struct {
  bit data[BUS64_WIDTH];
} bus64;

void bus64_setbit(bus64 *b, int i, bit v);
bit bus64_getbit(bus64 b, int i);
bus64 bus64_zero();
bus64 bus64_ones();

#endif
