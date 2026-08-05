  addi x1, x0, 10
  addi x2, x0, 5
  bge  x1, x2, greater_than
  addi x3, x0, 111
greater_than:
  addi x3, x0, 222
