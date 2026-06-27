#include <assert.h>
#include "counter.h"
#include "utilities.h"
#include "decoder.h"

void test_counter() {
  counter64 c;
  bus64 result;
  counter_init(&c);
  
  // counter == 0
  result = register64_output(&c.output_reg);
  assert(is_equal_bus_uint(result, 0ULL) == 1);

  // counter 3 ticks;
  counter_tick(&c);
  counter_tick(&c);
  counter_tick(&c);
  result = register64_output(&c.output_reg);
  assert(is_equal_bus_uint(result, 3ULL) == 1);

  // load 512
  c.count_in = encode_amount(512ULL);
  c.load = 1;
  counter_tick(&c);
  c.load = 0;
  result = register64_output(&c.output_reg);
  assert(is_equal_bus_uint(result, 512ULL) == 1);

  // clear
  c.clear = 1;
  counter_tick(&c);
  result = register64_output(&c.output_reg);
  assert(is_equal_bus_uint(result, 0ULL) == 1);
}

int main() {
  test_counter();
}
