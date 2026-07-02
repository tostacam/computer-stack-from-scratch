#include <assert.h>
#include "utilities.h"
#include "register.h"
#include "ram.h"

void test_ram() {
  RAM ram;
  RAM_init(&ram);

  // w[0:7] = UINT64_MAX
  ram.address = 0ULL;
  ram.write_data = UINT64_MAX;
  ram.write_enable = 1;
  RAM_write(&ram, MEM_DWRD_SIZE);
  ram.read_enable = 1;
  RAM_read(&ram, MEM_DWRD_SIZE);
  assert(ram.read_data == UINT64_MAX);

  // w[63] = 5
  ram.address = 63ULL;
  ram.write_data = 5ULL;
  ram.write_enable = 1;
  RAM_write(&ram, MEM_BYTE_SIZE);
  ram.read_enable = 1;
  RAM_read(&ram, MEM_BYTE_SIZE);
  assert(ram.read_data == 5ULL);

  // w[0:7] = UINT32_MAX
  ram.address = 4;
  ram.write_data = 0;
  ram.write_enable = 1;
  RAM_write(&ram, MEM_WORD_SIZE);
  ram.address = 0;
  ram.read_enable = 1;
  RAM_read(&ram, MEM_DWRD_SIZE);
  assert(ram.read_data == UINT32_MAX);
}

int main() {
  test_ram();
}
