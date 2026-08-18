  addi x1, x0, 5
  addi x2, x0, 10
  bne x1, x2, notequal
  addi x3, x0, 111
notequal:
  addi x3, x0, 222
  ebreak
