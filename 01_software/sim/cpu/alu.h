#ifndef ALU_H
#define ALU_H

// circuits
#include "adders.h"
#include "bus.h"
#include "multiplexers.h"
#include "bitwise.h"
#include "shifters.h"

#define CONTROL_BITS  4
#define OUTPUT_FLAGS  4

enum alu_op {
  ADD,
  SUB,
  NOT,
  AND,
  OR,
  XOR,
  SHL,
  SHR
};

typedef struct {
  bus64 output;
  bit  zero;
  bit carry;
  bit overflow;
  bit negative;
} alu_result;

alu_result alu_execute(bus64 a, bus64 b, enum alu_op op);

#endif
