#include <assert.h>
#include <stdint.h>
#include "utilities.h"
#include "alu.h"

#include <stdio.h>

void test_alu_add() {
  bus64       a, b;
  alu_result  res;
  uint64_t    res_uint;

  // 1 + 2
  a = encode_amount(1ULL);
  b = encode_amount(2ULL);
  res = alu_execute(a, b, ALU_OP_ADD);
  res_uint = decode_amount(res.output);
  assert(res_uint == 3ULL);

  // 0 + 0
  a = encode_amount(0ULL);
  b = encode_amount(0ULL);
  res = alu_execute(a, b, ALU_OP_ADD);
  res_uint = decode_amount(res.output);
  assert(res_uint == 0ULL);

  // 20 + 42
  a = encode_amount(20ULL);
  b = encode_amount(42ULL);
  res = alu_execute(a, b, ALU_OP_ADD);
  res_uint = decode_amount(res.output);
  assert(res_uint == 62ULL);

  // 5 - 3
  a = encode_amount(5ULL);
  b = encode_amount(3ULL);
  res = alu_execute(a, b, ALU_OP_SUB);
  res_uint = decode_amount(res.output);
  assert(res_uint == 2ULL);
}

void test_alu_logic() {
  bus64       a, b;
  alu_result  res;
  uint64_t    res_uint;

  // 1110 & 1011 = 1010
  a = encode_amount(14ULL);
  b = encode_amount(11ULL);
  res = alu_execute(a, b, ALU_OP_AND);
  res_uint = decode_amount(res.output);
  assert(res_uint == 10ULL);
  // 0000 & 0000 = 0000
  a = encode_amount(0ULL);
  b = encode_amount(0ULL);
  res = alu_execute(a, b, ALU_OP_AND);
  res_uint = decode_amount(res.output);
  assert(res_uint == 0ULL);
  // 1110 | 1011 = 1111
  a = encode_amount(14ULL);
  b = encode_amount(11ULL);
  res = alu_execute(a, b, ALU_OP_OR);
  res_uint = decode_amount(res.output);
  assert(res_uint == 15ULL);
  // 0000 | 0000 = 0000
  a = encode_amount(0ULL);
  b = encode_amount(0ULL);
  res = alu_execute(a, b, ALU_OP_OR);
  res_uint = decode_amount(res.output);
  assert(res_uint == 0ULL);
  // 1110 ^ 1011 = 0101
  a = encode_amount(14ULL);
  b = encode_amount(11ULL);
  res = alu_execute(a, b, ALU_OP_XOR);
  res_uint = decode_amount(res.output);
  assert(res_uint == 5ULL);
  // !(0) = 1
  a = encode_amount(0ULL);
  b = encode_amount(0ULL);
  res = alu_execute(a, b, ALU_OP_NOT);
  res_uint = decode_amount(res.output);
  assert(res_uint == UINT64_MAX);
  // !(1) = 0 
  a = encode_amount(UINT64_MAX);
  b = encode_amount(0ULL);
  res = alu_execute(a, b, ALU_OP_NOT);
  res_uint = decode_amount(res.output);
  assert(res_uint == 0ULL);
}

void test_alu_shifts() {
  bus64       a, b;
  alu_result  res;
  uint64_t    res_uint;

  // 9 << 2 = 36
  a = encode_amount(9ULL);
  b = encode_amount(2ULL);
  res = alu_execute(a, b, ALU_OP_SHL);
  res_uint = decode_amount(res.output);
  assert(res_uint == 36ULL);
  // 8 >> 2 = 2
  a = encode_amount(8ULL);
  b = encode_amount(2ULL);
  res = alu_execute(a, b, ALU_OP_SHR);
  res_uint = decode_amount(res.output);
  assert(res_uint == 2ULL);
}

int main() {
  // simple ops tests
  test_alu_add();
  test_alu_logic();
  test_alu_shifts();
}
