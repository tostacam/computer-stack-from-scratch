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

Supported operations:

- ADD
- SUB
- AND
- OR
- XOR
- NOT
- SHL
- SHR

ALU returns:
- result (64-bit)
- flags:
  - Z (zero)
  - C (carry)
  - O (overflow, optional early phase)
