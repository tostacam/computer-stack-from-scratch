#include <assert.h>
#include "register_file.h"
#include "decoder_encoder.h"
#include "utilities.h"
#include <stdio.h>

void test_register_file() {
  register_file rf;
  register_file_init(&rf);

  // x0 = x1 = ... = x31 = 0
  for (int i = 0; i < REGISTER_FILE_SIZE; ++i) {
    assert(decode_amount(register64_output(&rf.register_data[i])) == 0ULL); 
  }

  // x1 = 20, x31 = 40
  rf.write_addr = encode_amount(1ULL);
  rf.write_data = encode_amount(20ULL);
  rf.write_enable = 1;
  register_file_tick(&rf);

  rf.write_addr = encode_amount(31ULL);
  rf.write_data = encode_amount(40ULL);
  rf.write_enable = 1;
  register_file_tick(&rf);

  rf.read_addr_a = encode_amount(1ULL);
  rf.read_addr_b = encode_amount(31ULL);
  register_file_eval(&rf);

  /*
  for (int i = 0; i < REGISTER_FILE_SIZE; i = i + 4) {
    printf("x%d: %llu, ", i, decode_amount(register64_output(&rf.register_data[i])));
    printf("x%d: %llu, ", i + 1, decode_amount(register64_output(&rf.register_data[i + 1])));
    printf("x%d: %llu, ", i + 2, decode_amount(register64_output(&rf.register_data[i + 2])));
    printf("x%d: %llu\n", i + 3, decode_amount(register64_output(&rf.register_data[i + 3])));
  }
  */

  assert(is_equal_bus_uint(rf.read_data_a, 20ULL) == 1);
  assert(is_equal_bus_uint(rf.read_data_b, 40ULL) == 1);

  // x0 == 0 always (RISC-V)
  rf.write_addr = encode_amount(0ULL);
  rf.write_data = encode_amount(1ULL);
  rf.write_enable = 1;
  register_file_tick(&rf);
  assert(decode_amount(register64_output(&rf.register_data[0])) == 0ULL); 
}

int main() {
  test_register_file();
}
