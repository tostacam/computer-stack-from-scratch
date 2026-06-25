#include "bitwise.h"

bus64 bitwise_nand(bus64 a, bus64 b) {
  bus64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&r, i, NAND(bus64_getbit(a,i),bus64_getbit(b,i)));
  }
  return r;
}

bus64 bitwise_not(bus64 a) {
  bus64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&r, i, NOT(bus64_getbit(a,i)));
  }
  return r;
}

bus64 bitwise_and(bus64 a, bus64 b) {
  bus64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&r, i, AND(bus64_getbit(a,i),bus64_getbit(b,i)));
  }
  return r;
}

bus64 bitwise_or(bus64 a, bus64 b) {
  bus64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&r, i, OR(bus64_getbit(a,i),bus64_getbit(b,i)));
  }
  return r;
}

bus64 bitwise_nor(bus64 a, bus64 b) {
  bus64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&r, i, NOR(bus64_getbit(a,i),bus64_getbit(b,i)));
  }
  return r;
}

bus64 bitwise_xor(bus64 a, bus64 b) {
  bus64 r;
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    bus64_setbit(&r, i, XOR(bus64_getbit(a,i),bus64_getbit(b,i)));
  }
  return r;
}
