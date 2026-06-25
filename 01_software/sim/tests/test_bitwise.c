#include <assert.h>
#include "bitwise.h"

void test_bitwise_nand(){
  bus64 r;
    
  r = bitwise_nand(bus64_zero(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }
  
  r = bitwise_nand(bus64_zero(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = bitwise_nand(bus64_ones(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = bitwise_nand(bus64_ones(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
}

void test_bitwise_not() {
  bus64 r;

  r = bitwise_not(bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = bitwise_not(bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
}

void test_bitwise_and() {
  bus64 r;

  r = bitwise_and(bus64_zero(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
  
  r = bitwise_and(bus64_zero(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }

  r = bitwise_and(bus64_ones(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }

  r = bitwise_and(bus64_ones(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }
}

void test_bitwise_or() {
  bus64 r;

  r = bitwise_or(bus64_zero(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
  
  r = bitwise_or(bus64_zero(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = bitwise_or(bus64_ones(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = bitwise_or(bus64_ones(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }
}

void test_bitwise_nor() {
  bus64 r;

  r = bitwise_nor(bus64_zero(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }
  
  r = bitwise_nor(bus64_zero(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }

  r = bitwise_nor(bus64_ones(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }

  r = bitwise_nor(bus64_ones(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
}

void test_bitwise_xor() {
  bus64 r;

  r = bitwise_xor(bus64_zero(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
  
  r = bitwise_xor(bus64_zero(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = bitwise_xor(bus64_ones(), bus64_zero());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 1);
  }

  r = bitwise_xor(bus64_ones(), bus64_ones());
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(bus64_getbit(r, i) == 0);
  }
}

int main() {
  test_bitwise_nand();
  test_bitwise_not();
  test_bitwise_and();
  test_bitwise_or();
  test_bitwise_nor();
  test_bitwise_xor();
}
