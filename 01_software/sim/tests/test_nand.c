#include <assert.h>
#include "nand.h"

void test_nand() {
  assert(NAND(0,0) == 1);
  assert(NAND(0,1) == 1);
  assert(NAND(1,0) == 1);
  assert(NAND(1,1) == 0);
}

int main(int argc, char *argv[]) {
  test_nand();
}
