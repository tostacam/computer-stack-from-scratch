addi x1, x0, -1     # 0xFF
sb   x1, 5(x0)

addi x1, x0, 0x123  # 0x123
sw   x1, 0(x0)

lb   x2, 0(x0)
