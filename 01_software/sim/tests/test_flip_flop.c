#include <assert.h>
#include "flip-flop.h"

void test_flip_flop() {
  d_flip_flop ff;
  
  flip_flop_init(&ff);
  assert(ff.D == 0 && ff.Q == 0);

  // D = 0 -> Q = 0
  flip_flop_trigger(&ff);
  assert(ff.D == 0 && ff.Q == 0);

  // D = 1 -> Q = 1
  ff.D = 1;
  flip_flop_trigger(&ff);
  assert(ff.D == 1 && ff.Q == 1);
}

int main() {
  test_flip_flop();
}
