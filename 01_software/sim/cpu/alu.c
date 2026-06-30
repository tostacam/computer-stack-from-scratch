#include "alu.h"

void alu_init(ALU *alu) {
  alu->a          = bus64_zero();
  alu->b          = bus64_zero();
  alu->opcode     = ALU_OP_NOP;
  alu->output     = bus64_zero();
  alu->f_zero     = 0;
  alu->f_carry    = 0;
  alu->f_overflow = 0;
  alu->f_negative = 0;
}

void alu_tick(ALU *alu) {
  add64_result  r;
  bit           sign_a, sign_b, sign_r;

  switch(alu->opcode) {
    case ALU_OP_NOP: {
      break;                 
    }
    case ALU_OP_ADD: {
      r = add64(alu->a, alu->b, 0);
      sign_a = bus64_getbit(alu->a, BUS64_WIDTH - 1);
      sign_b = bus64_getbit(alu->b, BUS64_WIDTH - 1);
      sign_r = bus64_getbit(r.bus64_sum, BUS64_WIDTH - 1);

      alu->output     = r.bus64_sum;
      alu->f_zero     = is_zero64(r.bus64_sum);
      alu->f_carry    = r.carry_out;
      alu->f_overflow = (sign_a == sign_b) && (sign_r != sign_a);
      alu->f_negative = bus64_getbit(r.bus64_sum, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_SUB: {
      r = add64(alu->a, bitwise_not(alu->b), 1);
      sign_a = bus64_getbit(alu->a, BUS64_WIDTH - 1);
      sign_b = bus64_getbit(alu->b, BUS64_WIDTH - 1);
      sign_r = bus64_getbit(r.bus64_sum, BUS64_WIDTH - 1);

      alu->output     = r.bus64_sum;
      alu->f_zero     = is_zero64(r.bus64_sum);
      alu->f_carry    = r.carry_out;
      alu->f_overflow = (sign_a != sign_b) && (sign_r != sign_a);
      alu->f_negative = sign_r;
      break;
    }
    case ALU_OP_NOT: {
      alu->output     = bitwise_not(alu->a);
      alu->f_zero     = is_zero64(alu->output);
      alu->f_carry    = 0;
      alu->f_overflow = 0;
      alu->f_negative = bus64_getbit(alu->output, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_AND: {
      alu->output     = bitwise_and(alu->a, alu->b);
      alu->f_zero     = is_zero64(alu->output);
      alu->f_carry    = 0;
      alu->f_overflow = 0;
      alu->f_negative = bus64_getbit(alu->output, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_OR: {
      alu->output     = bitwise_or(alu->a, alu->b);
      alu->f_zero     = is_zero64(alu->output);
      alu->f_carry    = 0;
      alu->f_overflow = 0;
      alu->f_negative = bus64_getbit(alu->output, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_XOR: {
      alu->output     = bitwise_xor(alu->a, alu->b);
      alu->f_zero     = is_zero64(alu->output);
      alu->f_carry    = 0;
      alu->f_overflow = 0;
      alu->f_negative = bus64_getbit(alu->output, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_SHL: {
      alu->output     = shift_left(alu->a, decode_amount(alu->b));
      alu->f_zero     = is_zero64(alu->output);
      alu->f_carry    = 0;
      alu->f_overflow = 0;
      alu->f_negative =bus64_getbit(alu->output, BUS64_WIDTH - 1); 
      break;
    }
    case ALU_OP_SHR: {
      alu->output     = shift_right(alu->a, decode_amount(alu->b));
      alu->f_zero     = is_zero64(alu->output);
      alu->f_carry    = 0;
      alu->f_overflow = 0;
      alu->f_negative = bus64_getbit(alu->output, BUS64_WIDTH - 1); 
      break;
    }
    default: {
      break;
    }
  }
}
