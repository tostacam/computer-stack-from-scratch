#include <assert.h>
#include "flip-flop.h"

void test_flip_flop() {
  bit q_output;
  
  // default state
  d_flip_flop ff = flip_flop_init();
  q_output = flip_flop_output(&ff);
  assert(ff.D == 0 && q_output == 0);

  // D = 0 -> Q = 0
  flip_flop_trigger(&ff);
  q_output = flip_flop_output(&ff);
  assert(ff.D == 0 && q_output == 0);

  // D = 1 -> Q = 1
  flip_flop_input(&ff, 1);
  flip_flop_trigger(&ff);
  q_output = flip_flop_output(&ff);
  assert(ff.D == 1 && q_output == 1);
}

int main() {
  test_flip_flop();
}
