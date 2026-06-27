#include <assert.h>
#include "program_counter.h"
#include "decoder_encoder.h"
#include "utilities.h"

void test_program_counter() {
  program_counter pc;
  bus64 result;
  program_counter_init(&pc);

  // pc = 0
  result = register64_output(&pc.output_reg);
  assert(is_equal_bus_uint(result, 0ULL) == 1);

  // pc 3 ticks
  program_counter_tick(&pc);
  program_counter_tick(&pc);
  program_counter_tick(&pc);
  result = register64_output(&pc.output_reg);
  assert(is_equal_bus_uint(result, 12ULL) == 1);

  // jump to 512
  pc.jump_addr = encode_amount(512ULL);
  pc.jump = 1;
  program_counter_tick(&pc);
  pc.jump = 0;
  result = register64_output(&pc.output_reg);
  assert(is_equal_bus_uint(result, 512ULL) == 1);

  // clear
  pc.clear = 1;
  program_counter_tick(&pc);
  pc.clear = 0;
  result = register64_output(&pc.output_reg);
  assert(is_equal_bus_uint(result, 0ULL) == 1);
}

int main() {
  test_program_counter();  
}
