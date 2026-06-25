#ifndef ADDERS_H
#define ADDERS_H

#include "gates.h"

typedef struct {
  bit sum;
  bit carry;
} HalfAdder;

HalfAdder half_adder(bit a, bit b);

typedef struct {
  bit sum;
  bit carry;
} FullAdder;

FullAdder full_adder(bit a, bit b, bit cin);

#endif
