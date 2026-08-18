addi x1, x0, -1
sb   x1, 1(x0)
addi x1, x0, 42
sb   x1, 0(x0)
lbu  x2, 1(x0)
ebreak
