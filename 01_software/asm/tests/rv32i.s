# R-type
add   x1, x2, x3
sub   x4, x5, x6
sll   x7, x8, x9
slt   x10, x11, x12
sltu  x13, x14, x15
xor   x16, x17, x18
srl   x19, x20, x21
sra   x22, x23, x24
or    x25, x26, x27
and   x28, x29, x30

# I-type
addi  x1, x2, 123
slti  x3, x4, -1
sltiu x5, x6, 7
xori  x7, x8, 0xff
ori   x9, x10, 42
andi  x11, x12, 255

slli  x13, x14, 3
srli  x15, x16, 4
srai  x17, x18, 5

lb    x19, 0(x20)
lh    x21, 2(x22)
lw    x23, 4(x24)
lbu   x25, 8(x26)
lhu   x27, 10(x28)

jalr  x29, 0(x30)

# S-type
sb    x1, 0(x2)
sh    x3, 2(x4)
sw    x5, 4(x6)

# B-type
# U-type
# J-type
