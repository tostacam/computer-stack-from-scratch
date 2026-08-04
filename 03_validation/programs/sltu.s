addi x1, x0, -1   # 0xFFFFFFFFFFFFFFFF
addi x2, x0, 1    
sltu x3, x2, x1   # 1 < 0xFFFFFFFFFFFFFFFF (unsigned) -> 1
