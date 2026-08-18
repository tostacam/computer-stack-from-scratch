  addi x1, x0, -1
  addi x2, x0, 1
  bgeu x1, x2, greater_than_unsigned
  addi x3, x0, 111
greater_than_unsigned:
  addi x3, x0, 222
  ebreak
