# Single-Cycle CPU Control Signals

## Control Signal Effecs When Deasserted/Asserted

| Signal | Deasserted | Asserted |
|---------|------|-------------|
| RegWrite | None. | Write register input is writen with the value on the write data input. |
| ALUSrc | Second ALU operand comes from the second register fiel output (rd2). | Second ALU operand is the 12bits of the immediate instruction |
| PCSrc/Branch | PC is replaced by the output of the adder that computes the value of PC+4 | The PC is replaced by the output of the adder that computes the branch target. |
| MemRead | None. | Use input address to read data from  memory.  |
| MemWrite | None. | use input address to write data to memory. |
| MemToReg | Register write data comes from the ALU output. | Register write data comes from the data in memory. |

---

## Control Truth Table

| Instruction | ALUSrc | MemtoReg | RegWrite | MemReead | MemWrite | Branch | ALUOp |
|-------------|:--------:|:------:|:-------:|:--------:|:--------:|:------:|:-----:|
| R-Type | 0 | 0 | 1 | 0 | 0 | 0 | 10 |
| lw | 1 | 1 | 1 | 1 | 0 | 0 | 00 |
| sw | 1 | X | 0 | 0 | 1 | 0 | 00 |
| beq | 0 | X | 0 | 0 | 0 | 1 | 01 |

---

## ALUOp Encoding

| ALUOp | Meaning |
|:-----:|---------|
| 00 | Add (address calculation) |
| 01 | Subtract / Compare (branches) |
| 10 | Decode funct3/funct7 |
| 11 | Reserved |

---

## PC Source

| Branch | Zero | Next PC |
|:------:|:----:|---------|
| 0 | X | PC + 4 |
| 1 | 0 | PC + 4 |
| 1 | 1 | Branch Target |
