#include "gates.h"

bit NOT(bit a) {
  return NAND(a, a);
}

bit AND(bit a, bit b) {
  return NAND(NAND(a, b), NAND(a, b));
}

bit OR(bit a, bit b) {
  return NAND(NOT(a), NOT(b));
}

bit NOR(bit a, bit b) {
  return NOT(OR(a, b));
}

bit XOR(bit a, bit b) {
  bit t1 = AND(a, NOT(b));
  bit t2 = AND(NOT(a), b);
  return OR(t1, t2);
}
