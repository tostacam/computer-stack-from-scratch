#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include "decoder_encoder.h"
#include "counter.h"
#include "register.h"
#include "multiplexers.h"

#define INSTRUCTION_WIDTH 4

typedef struct {
  bus64       count_inc;
  bus64       jump_addr;
  bit         clear;
  bit         jump;
  register64  output_reg;
} program_counter;

void program_counter_init(program_counter *pc);
void program_counter_tick(program_counter *pc);

#endif
