lui  x1, 0x4030
addi x1, x1, 0x210
sw   x1, 4(x0)
addi x2, x0, -1
sb   x2, 0(x0)
lw   x3, 4(x0)
ebreak
