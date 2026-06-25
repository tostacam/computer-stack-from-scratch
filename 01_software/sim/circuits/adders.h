#ifndef ADDERS_H
#define ADDERS_H

#include "gates.h"
#include "bus.h"

typedef struct {
  bit sum;
  bit carry_out;
} HalfAdder;

HalfAdder half_adder(bit a, bit b);

typedef struct {
  bit sum;
  bit carry_out;
} FullAdder;

FullAdder full_adder(bit a, bit b, bit cin);

typedef struct {
  bus64 sum;
  bit carry_out;
} add64_result;

add64_result add64(64BitAdder a, 64BitAdder b, bit cin);

#endif
