#include <assert.h>
#include "gates.h"

void test_not() {
  assert(NOT(0) == 1);
  assert(NOT(1) == 0);
}

void test_and() {
  assert(AND(0,0) == 0);
  assert(AND(0,1) == 0);
  assert(AND(1,0) == 0);
  assert(AND(1,1) == 1);
}

void test_or() {
  assert(OR(0,0) == 0);
  assert(OR(0,1) == 1);
  assert(OR(1,0) == 1);
  assert(OR(1,1) == 1);
}

void test_nor() {
  assert(NOR(0,0) == 1);
  assert(NOR(0,1) == 0);
  assert(NOR(1,0) == 0);
  assert(NOR(1,1) == 0);
}

void test_xor() {
  assert(XOR(0,0) == 0);
  assert(XOR(0,1) == 1);
  assert(XOR(1,0) == 1);
  assert(XOR(1,1) == 0);
}

int main() {
  test_not();
  test_and();
  test_or();
  test_nor();
  test_xor();
}
