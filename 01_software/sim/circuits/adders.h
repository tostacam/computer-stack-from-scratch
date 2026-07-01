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
  bus64 bus64_sum;
  bit carry_out;
} add64_result;

add64_result add64(bus64 a, bus64 b, bit cin);
bus64 add64_no_crry(bus64 a, bus64 b);

#endif
