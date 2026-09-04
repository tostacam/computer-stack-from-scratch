  addi x1, x0, 123
  lui  x2, 0x10
  sw   x1, 0(x2)
  lui  x2, 0x20000  # UART
  addi x2, x2, 0x10
  addi x1, x0, 65
  sb   x1, 0(x2)

loop:
  jal x0, loop
