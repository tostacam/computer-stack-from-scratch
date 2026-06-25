#ifndef BITWISE_H
#define BITWISE_H

#include "gates.h"
#include "bus.h"

bus64 bitwise_nand(bus64 a, bus64 b);
bus64 bitwise_not(bus64 a);
bus64 bitwise_and(bus64 a, bus64 b);
bus64 bitwise_or(bus64 a, bus64 b);
bus64 bitwise_nor(bus64 a, bus64 b);
bus64 bitwise_xor(bus64 a, bus64 b);

#endif
