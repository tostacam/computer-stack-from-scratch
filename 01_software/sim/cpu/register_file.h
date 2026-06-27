#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include "bit.h"
#include "bus.h"
#include "register.h"
#include "multiplexers.h"
#include "decoder_encoder.h"

#define REGISTER_FILE_SIZE  32

typedef struct {
  register64  register_data[REGISTER_FILE_SIZE];
  bus64       write_data;
  bus64       write_addr;
  bit         write_enable;
  bus64       read_addr_a;
  bus64       read_addr_b;
  bus64       read_data_a;
  bus64       read_data_b;
} register_file;

void register_file_init(register_file *rf);
void register_file_tick(register_file *rf);

#endif 
