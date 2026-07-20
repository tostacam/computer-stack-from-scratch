# Forward branch
start:
  addi x1, x0, 3
  beq  x1, x0, done
  addi x2, x0, 1
done:
  addi x3, x0, 2

# Backward branch (loop)
loop:
  addi x4, x4, -1
  bne  x4, x0, loop

# Forward jump
  jal  x5, function
  addi x6, x0, 0      # should be skipped
function:
  addi x7, x0, 42
  jalr x0, 0(x1)

# Multiple labels on one target
label1:
label2:
  addi x8, x0, 8

# Branch over several instructions
  beq x0, x0, end
  addi x9,  x0, 1
  addi x10, x0, 2
  addi x11, x0, 3
end:
  addi x12, x0, 4

# Small countdown loop
main:
  addi x13, x0, 5
count:
  addi x13, x13, -1
  bne  x13, x0, count
  jal x0, exit
exit:
  addi x0, x0, 0
