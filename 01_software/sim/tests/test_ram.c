#include <assert.h>
#include "utilities.h"
#include "register.h"
#include "ram.h"

void test_ram() {
  RAM64 ram;
  RAM64_init(&ram);

  // w0 = w1 = ... = w63 = 0
  for (int i = 0; i < RAM64_WORDS; ++i) {
    assert(is_equal_bus_uint(register64_output(&ram.register_data[i]), 0ULL));
  }

  // w0 = UINT64_MAX
  ram.address = encode_amount(0ULL);
  ram.write_data = encode_amount(UINT64_MAX);
  ram.write_enable = 1;
  RAM64_tick(&ram);
  ram.read_enable = 1;
  RAM64_read(&ram);
  assert(is_equal_bus_uint(ram.read_data, UINT64_MAX) == 1);

  // w63 = 5
  ram.address = encode_amount(63ULL);
  ram.write_data = encode_amount(5ULL);
  ram.write_enable = 1;
  RAM64_tick(&ram);
  ram.read_enable = 1;
  RAM64_read(&ram);
  assert(is_equal_bus_uint(ram.read_data, 5ULL) == 1);

  // w0 = 0
  ram.address = encode_amount(0ULL);
  ram.write_data = encode_amount(0ULL);
  ram.write_enable = 1;
  RAM64_tick(&ram);
  ram.read_enable = 1;
  RAM64_read(&ram);
  assert(is_equal_bus_uint(ram.read_data, 0ULL) == 1);
}

int main() {
  test_ram();
}
