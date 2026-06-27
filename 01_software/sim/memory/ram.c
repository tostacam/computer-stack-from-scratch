#include "ram.h"

void RAM64_init(RAM64 *ram) {
  for (int i = 0; i < RAM64_WORDS; ++i) {
    ram->register_data[i] = register64_init();
  }
  ram->address      = bus64_zero();
  ram->write_data   = bus64_zero();
  ram->write_enable = 0;
  ram->read_data    = bus64_zero();
  ram->read_enable  = 0;
}

void RAM64_tick(RAM64 *ram) {
  if (ram->write_enable == 1) {
    register64_enable(&ram->register_data[decode_amount(ram->address)]);
    register64_input(&ram->register_data[decode_amount(ram->address)], ram->write_data);
    ram->write_enable = 0;  
  }

  if (ram->read_enable == 1) {
    ram->read_data = register64_output(&ram->register_data[decode_amount(ram->address)]);
    ram->read_enable = 0;
  }

  // clock tick
  register64_trigger(&ram->register_data[decode_amount(ram->address)]);
  register64_disable(&ram->register_data[decode_amount(ram->address)]);
}
