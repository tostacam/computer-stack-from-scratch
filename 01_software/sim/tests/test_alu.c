#include <assert.h>
#include <stdint.h>
#include "utilities.h"
#include "alu.h"

#include <stdio.h>

void test_alu_add() {
  ALU       alu;
  uint64_t  res_uint;

  alu_init(&alu);

  // 1 + 2
  alu.a = encode_amount(1ULL);
  alu.b = encode_amount(2ULL);
  alu.opcode = ALU_OP_ADD;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 3ULL);

  // 0 + 0
  alu.a = encode_amount(0ULL);
  alu.b = encode_amount(0ULL);
  alu.opcode = ALU_OP_ADD;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  
  // 20 + 42
  alu.a = encode_amount(20ULL);
  alu.b = encode_amount(42ULL);
  alu.opcode = ALU_OP_ADD;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 62ULL);
  
  // 5 - 3
  alu.a = encode_amount(5ULL);
  alu.b = encode_amount(3ULL);
  alu.opcode = ALU_OP_SUB;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 2ULL);
}

void test_alu_logic() {
  ALU       alu;
  uint64_t  res_uint;

  alu_init(&alu);

  // 1110 & 1011 = 1010
  alu.a = encode_amount(14ULL);
  alu.b = encode_amount(11ULL);
  alu.opcode = ALU_OP_AND;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 10ULL);
  // 0000 & 0000 = 0000
  alu.a = encode_amount(0ULL);
  alu.b = encode_amount(0ULL);
  alu.opcode = ALU_OP_AND;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  // 1110 | 1011 = 1111
  alu.a = encode_amount(14ULL);
  alu.b = encode_amount(11ULL);
  alu.opcode = ALU_OP_OR;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 15ULL);
  // 0000 | 0000 = 0000
  alu.a = encode_amount(0ULL);
  alu.b = encode_amount(0ULL);
  alu.opcode = ALU_OP_OR;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  // 1110 ^ 1011 = 0101
  alu.a = encode_amount(14ULL);
  alu.b = encode_amount(11ULL);
  alu.opcode = ALU_OP_XOR;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 5ULL);
}

void test_alu_shifts() {
  ALU       alu;
  uint64_t  res_uint;

  alu_init(&alu);

  // SLL: 9 << 2 = 36
  alu.a = encode_amount(9ULL);
  alu.b = encode_amount(2ULL);
  alu.opcode = ALU_OP_SLL;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 36ULL);
  // SRL: 8 >> 2 = 2
  alu.a = encode_amount(8ULL);
  alu.b = encode_amount(2ULL);
  alu.opcode = ALU_OP_SRL;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 2ULL);
  
  // SRA: 16 >> 2 = 4
  alu.a = encode_amount(16ULL);
  alu.b = encode_amount(2ULL);
  alu.opcode = ALU_OP_SRA;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 4ULL);
  // SRA: -16 >> 2 = -4
  alu.a = encode_amount((uint64_t)(-16LL));
  alu.b = encode_amount(2ULL);
  alu.opcode = ALU_OP_SRA;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert((int64_t)res_uint == -4LL);
  // printf("%llu", res_uint);
}

void test_alu_slt() { 
  ALU       alu;
  uint64_t  res_uint;

  alu_init(&alu);

  // SLT: (5 < 10) = 1
  alu.a = encode_amount(5ULL);
  alu.b = encode_amount(10ULL);
  alu.opcode = ALU_OP_SLT;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 1ULL);
  // SLT: (10 < 5) = 0
  alu.a = encode_amount(10ULL);
  alu.b = encode_amount(5ULL);
  alu.opcode = ALU_OP_SLT;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  // SLT: (-1 < 1) = 1
  alu.a = encode_amount((int64_t)(-1LL));
  alu.b = encode_amount(1ULL);
  alu.opcode = ALU_OP_SLT;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 1ULL);
  // SLT: (1 < -1) = 0
  alu.a = encode_amount(1ULL);
  alu.b = encode_amount((int64_t)(-1LL));
  alu.opcode = ALU_OP_SLT;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 1ULL);

  // SLTU: (5 < 10) = 1
  alu.a = encode_amount(5ULL);
  alu.b = encode_amount(10ULL);
  alu.opcode = ALU_OP_SLT;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 1ULL);
  // SLTU: (10 < 5) = 0
  alu.a = encode_amount(10ULL);
  alu.b = encode_amount(5ULL);
  alu.opcode = ALU_OP_SLT;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  // SLTU: (UINT64_MAX < 0) = 1
  alu.a = encode_amount(UINT64_MAX);
  alu.b = encode_amount(0ULL);
  alu.opcode = ALU_OP_SLTU;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  // SLTU: (0 < UINT64_MAX) = 1
  alu.a = encode_amount(0ULL);
  alu.b = encode_amount(UINT64_MAX);
  alu.opcode = ALU_OP_SLTU;
  alu_eval(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 1ULL);
}

int main() {
  // simple ops tests
  test_alu_add();
  test_alu_logic();
  test_alu_shifts();
}
