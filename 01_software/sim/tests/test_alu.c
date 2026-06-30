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
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 3ULL);

  // 0 + 0
  alu.a = encode_amount(0ULL);
  alu.b = encode_amount(0ULL);
  alu.opcode = ALU_OP_ADD;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  
  // 20 + 42
  alu.a = encode_amount(20ULL);
  alu.b = encode_amount(42ULL);
  alu.opcode = ALU_OP_ADD;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 62ULL);
  
  // 5 - 3
  alu.a = encode_amount(5ULL);
  alu.b = encode_amount(3ULL);
  alu.opcode = ALU_OP_SUB;
  alu_tick(&alu);
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
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 10ULL);
  // 0000 & 0000 = 0000
  alu.a = encode_amount(0ULL);
  alu.b = encode_amount(0ULL);
  alu.opcode = ALU_OP_AND;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  // 1110 | 1011 = 1111
  alu.a = encode_amount(14ULL);
  alu.b = encode_amount(11ULL);
  alu.opcode = ALU_OP_OR;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 15ULL);
  // 0000 | 0000 = 0000
  alu.a = encode_amount(0ULL);
  alu.b = encode_amount(0ULL);
  alu.opcode = ALU_OP_OR;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
  // 1110 ^ 1011 = 0101
  alu.a = encode_amount(14ULL);
  alu.b = encode_amount(11ULL);
  alu.opcode = ALU_OP_XOR;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 5ULL);
  // !(0) = 1
  alu.a = encode_amount(0ULL);
  alu.opcode = ALU_OP_NOT;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == UINT64_MAX);
  // !(1) = 0 
  alu.a = encode_amount(UINT64_MAX);
  alu.opcode = ALU_OP_NOT;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 0ULL);
}

void test_alu_shifts() {
  ALU       alu;
  uint64_t  res_uint;

  alu_init(&alu);

  // 9 << 2 = 36
  alu.a = encode_amount(9ULL);
  alu.b = encode_amount(2ULL);
  alu.opcode = ALU_OP_SHL;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 36ULL);
  // 8 >> 2 = 2
  alu.a = encode_amount(8ULL);
  alu.b = encode_amount(2ULL);
  alu.opcode = ALU_OP_SHR;
  alu_tick(&alu);
  res_uint = decode_amount(alu.output);
  assert(res_uint == 2ULL);
}

int main() {
  // simple ops tests
  test_alu_add();
  test_alu_logic();
  test_alu_shifts();
}
