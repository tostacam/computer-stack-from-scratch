# Memory Map

The system uses a simple memory-mapped architecture where all peripherals are accessed through the CPU's address space.

| Address Range | Size | Device | Description |
|---------------|------|--------|-------------|
| `0x00000000` - `0x0000FFFF` | 64 KB | Boot ROM | Stores the bootloader and program code. Reset vector points here. |
| `0x00010000` - `0x0001FFFF` | 64 KB | RAM | General-purpose read/write memory for stack, data, and heap. |
| `0x20000000` | 4 B | GPIO | General-purpose input/output register. |
| `0x20000010` | 4 B | UART TX | UART transmit register. Writing a byte sends it over the serial interface. |
| `0x20000014` | 4 B | UART RX | UART receive register. Reading returns the next received byte. |

## Notes

- The CPU has a **32-bit address space**, but only a small portion is currently used.
- Boot execution begins at **`0x00000000`**.
- Peripherals are **memory-mapped**, meaning they are accessed with normal `lw`/`sw` instructions.
- Additional devices (timers, interrupts, SPI, etc.) can be added later.
