#include "alu.h"

alu_result alu_execute(bus64 a, bus64 b, enum alu_op op) {
  add64_result r;
  bit sign_a, sign_b, sign_r;
  alu_result res;

  switch(op) {
    case ALU_OP_ADD: {
      r = add64(a, b, 0);
      sign_a = bus64_getbit(a, BUS64_WIDTH - 1);
      sign_b = bus64_getbit(b, BUS64_WIDTH - 1);
      sign_r = bus64_getbit(r.bus64_sum, BUS64_WIDTH - 1);

      res.output    = r.bus64_sum;
      res.zero      = is_zero64(r.bus64_sum);
      res.carry     = r.carry_out;
      res.overflow  = (sign_a == sign_b) && (sign_r != sign_a);
      res.negative  = bus64_getbit(r.bus64_sum, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_SUB: {
      r = add64(a, bitwise_not(b), 1);
      sign_a = bus64_getbit(a, BUS64_WIDTH - 1);
      sign_b = bus64_getbit(b, BUS64_WIDTH - 1);
      sign_r = bus64_getbit(r.bus64_sum, BUS64_WIDTH - 1);

      res.output    = r.bus64_sum;
      res.zero      = is_zero64(r.bus64_sum);
      res.carry     = r.carry_out;
      res.overflow  = (sign_a != sign_b) && (sign_r != sign_a);
      res.negative  = sign_r;
      break;
    }
    case ALU_OP_NOT: {
      res.output    = bitwise_not(a);
      res.zero      = is_zero64(res.output);
      res.carry     = 0;
      res.overflow  = 0;
      res.negative  = bus64_getbit(res.output, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_AND: {
      res.output    = bitwise_and(a, b);
      res.zero      = is_zero64(res.output);
      res.carry     = 0;
      res.overflow  = 0;
      res.negative  = bus64_getbit(res.output, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_OR: {
      res.output    = bitwise_or(a, b);
      res.zero      = is_zero64(res.output);
      res.carry     = 0;
      res.overflow  = 0;
      res.negative  = bus64_getbit(res.output, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_XOR: {
      res.output    = bitwise_xor(a, b);
      res.zero      = is_zero64(res.output);
      res.carry     = 0;
      res.overflow  = 0;
      res.negative  = bus64_getbit(res.output, BUS64_WIDTH - 1);
      break;
    }
    case ALU_OP_SHL: {
      res.output    = shift_left(a, decode_amount(b));
      res.zero      = is_zero64(res.output);
      res.carry     = 0;
      res.overflow  = 0;
      res.negative  = bus64_getbit(res.output, BUS64_WIDTH - 1); 
      break;
    }
    case ALU_OP_SHR: {
      res.output    = shift_right(a, decode_amount(b));
      res.zero      = is_zero64(res.output);
      res.carry     = 0;
      res.overflow  = 0;
      res.negative  = bus64_getbit(res.output, BUS64_WIDTH - 1); 
      break;
    }
    default: {
      break;
    }
  }

  return res;
}
