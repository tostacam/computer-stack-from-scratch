# CPU Architecture Spec (C - RTL Contract)

Establishing the shared execution contract between the C CPU sim (SW model) and the Verilog RTL (HW model).

- C model = reference CPU behavior
- RTL model = hardware implementation
- This spec ensures both stay aligned instruction-by-instruction

---

## 1. Architectural State

Both models must expose the same CPU state:

- 32 × 64-bit general purpose registers (`x0` hardwired to 0, RISC-V)
- 64-bit program counter (`pc`)
- Main memory (byte-addressed, size defined per implementation)

---

## 2. Execution Model

Single-cycle execution:

1. Fetch instruction
2. Decode
3. Execute ALU / memory operation
4. Writeback
5. Update PC

Each instruction produces exactly one architectural state update.

---

## 3. ALU Contract

| ALU Opcode | Behavior | Used By |
|:----------:|----------|---------|
| `ALU_OP_ADD` | `result = a + b` | `add`, `addi`, `lw`, `sw`, `jalr`, `auipc` |
| `ALU_OP_SUB` | `result = a - b` | `sub`, `beq`, `bne` |
| `ALU_OP_AND` | `result = a & b` | `and`, `andi` |
| `ALU_OP_OR` | `result = a \| b` | `or`, `ori` |
| `ALU_OP_XOR` | `result = a ^ b` | `xor`, `xori` |
| `ALU_OP_SLL` | `result = a << (b & 0x3F)` | `sll`, `slli` |
| `ALU_OP_SRL` | `result = a >> (b & 0x3F)` (logical) | `srl`, `srli` |
| `ALU_OP_SRA` | `result = (int64_t)a >> (b & 0x3F)` | `sra`, `srai` |
| `ALU_OP_SLT` | `result = ((int64_t)a < (int64_t)b) ? 1 : 0` | `slt`, `slti`, `blt`, `bge` |
| `ALU_OP_SLTU` | `result = (a < b) ? 1 : 0` | `sltu`, `sltiu`, `bltu`, `bgeu` |
| `ALU_OP_PASS_B` | `result = b` | `lui` |
