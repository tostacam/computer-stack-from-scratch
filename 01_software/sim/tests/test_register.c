#include <assert.h>
#include "register.h"

void test_register() {
  register64  r64;
  bus64       q_output;

  // test initial state
  r64 = register64_init();
  q_output = register64_output(&r64);
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(q_output.data[i] == 0);
  } 

  // D = 0 -> Q = 0
  register64_trigger(&r64);
  q_output = register64_output(&r64);
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(q_output.data[i] == 0);
  }

  // D = 1, load = 0 -> Q = 0
  register64_input(&r64, bus64_ones());
  register64_trigger(&r64);
  q_output = register64_output(&r64);
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(q_output.data[i] == 0);
  }

  // D = 1, load = 1 -> Q = 1
  register64_enable(&r64);
  register64_trigger(&r64);
  register64_disable(&r64);
  q_output = register64_output(&r64);
  for (int i = 0; i < BUS64_WIDTH; ++i) {
    assert(q_output.data[i] == 1);
  }
}

int main() {
  test_register();
}
