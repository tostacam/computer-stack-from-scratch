#ifndef ALU_H
#define ALU_H

// circuits
#include "adders.h"
#include "bus.h"
#include "multiplexers.h"
#include "bitwise.h"
#include "shifters.h"
#include "comparators.h"
#include "decoder_encoder.h"

enum alu_op {
  ALU_OP_NOP,
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
  // input
  bus64       a;
  bus64       b;
  enum alu_op opcode;

  // output
  bus64 output;
  bit   f_zero; 
} ALU;

void alu_init(ALU *alu);
void alu_tick(ALU *alu);

#endif
