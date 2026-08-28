addi x1, x0, 42
lui x2, 0x10
sw x1, 0(x2)
lw x3, 0(x2)
ebreak
