#ifndef ALU_H
#define ALU_H

// circuits
#include "adders.h"
#include "bus.h"
#include "multiplexers.h"
#include "bitwise.h"
#include "shifters.h"
#include "comparators.h"
#include "decoder.h"

enum alu_op {
  ALU_OP_ADD,
  ALU_OP_SUB,
  ALU_OP_NOT,
  ALU_OP_AND,
  ALU_OP_OR,
  ALU_OP_XOR,
  ALU_OP_SHL,
  ALU_OP_SHR
};

typedef struct {
  bus64 output;
  bit   zero;
  bit   carry;
  bit   overflow;
  bit   negative;
} alu_result;

alu_result alu_execute(bus64 a, bus64 b, enum alu_op op);

#endif
