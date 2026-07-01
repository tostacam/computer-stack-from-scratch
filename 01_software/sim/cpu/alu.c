#include "alu.h"

void alu_init(ALU *alu) {
  alu->a          = bus64_zero();
  alu->b          = bus64_zero();
  alu->opcode     = ALU_OP_NOP;
  alu->output     = bus64_zero();
  alu->f_zero     = 0;
}

void alu_eval(ALU *alu) {
  add64_result  r;

  switch(alu->opcode) {
    case ALU_OP_NOP: {
      break;                 
    }
    case ALU_OP_ADD: {
      r = add64(alu->a, alu->b, 0);

      alu->output     = r.bus64_sum;
      alu->f_zero     = is_zero64(r.bus64_sum);
      break;
    }
    case ALU_OP_SUB: {
      r = add64(alu->a, bitwise_not(alu->b), 1);

      alu->output     = r.bus64_sum;
      alu->f_zero     = is_zero64(r.bus64_sum);
      break;
    }
    case ALU_OP_NOT: {
      alu->output     = bitwise_not(alu->a);
      alu->f_zero     = is_zero64(alu->output);
      break;
    }
    case ALU_OP_AND: {
      alu->output     = bitwise_and(alu->a, alu->b);
      alu->f_zero     = is_zero64(alu->output);
      break;
    }
    case ALU_OP_OR: {
      alu->output     = bitwise_or(alu->a, alu->b);
      alu->f_zero     = is_zero64(alu->output);
      break;
    }
    case ALU_OP_XOR: {
      alu->output     = bitwise_xor(alu->a, alu->b);
      alu->f_zero     = is_zero64(alu->output);
      break;
    }
    case ALU_OP_SHL: {
      alu->output     = shift_left(alu->a, decode_amount(alu->b));
      alu->f_zero     = is_zero64(alu->output);
      break;
    }
    case ALU_OP_SHR: {
      alu->output     = shift_right(alu->a, decode_amount(alu->b));
      alu->f_zero     = is_zero64(alu->output);
      break;
    }
    default: {
      break;
    }
  }
}
