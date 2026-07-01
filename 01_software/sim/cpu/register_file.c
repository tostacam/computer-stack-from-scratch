#include "register_file.h"

void register_file_init(register_file *rf) {
  for (int i = 0; i < REGISTER_FILE_SIZE; ++i) {
    rf->register_data[i] = register64_init();
  }
  rf->write_data    = bus64_zero();
  rf->write_addr    = bus64_zero();
  rf->write_enable  = 0;
  rf->read_addr_a   = bus64_zero();
  rf->read_addr_b   = bus64_zero();
  rf->read_data_a   = bus64_zero();
  rf->read_data_b   = bus64_zero();
}

void register_file_eval(register_file *rf) {
  uint64_t a_addr = decode_amount(rf->read_addr_a);
  uint64_t b_addr = decode_amount(rf->read_addr_b);
  
  rf->read_data_a = register64_output(&rf->register_data[a_addr]);
  rf->read_data_b = register64_output(&rf->register_data[b_addr]);
}

void register_file_tick(register_file *rf) {
  uint64_t wr_addr = decode_amount(rf->write_addr);

  if (rf->write_enable == 1) {
    // x0 always 0 (RISC-V)
    if (wr_addr != 0) {
      register64_enable(&rf->register_data[wr_addr]);
      register64_input(&rf->register_data[wr_addr], rf->write_data);
    }
    rf->write_enable = 0;
  }  

  // clock tick
  register64_trigger(&rf->register_data[wr_addr]);
  register64_disable(&rf->register_data[wr_addr]);
}
