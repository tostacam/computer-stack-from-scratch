  addi x1, x0, 1
  addi x2, x0, -1
  bltu x1, x2, less_than_unsigned
  addi x3, x0, 111
less_than_unsigned:
  addi x3, x0, 222
  ebreak
