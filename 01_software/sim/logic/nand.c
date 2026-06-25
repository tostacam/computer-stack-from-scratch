#include "nand.h"

bit NAND(bit a, bit b) {
  // NAND is the most fundamental gate, the base for everything
  return a && b ? 0 : 1;
}
