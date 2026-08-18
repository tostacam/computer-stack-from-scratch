addi x1, x0, -1
sh   x1, 2(x0)
addi x2, x0, 0x123
sh   x2, 0(x0)
lh   x3, 0(x0)
lh   x4, 2(x0)
ebreak
