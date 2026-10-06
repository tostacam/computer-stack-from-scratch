check = 0x10

.data
counter: .word 0
small_value: .half 0x01234
flag: .byte 1

.bss
buffer: .space 16

.text
start:
  addi x3, x0, check
  addi x4, x0, counter
  addi x5, x0, buffer

loop:
  jal x0, loop
