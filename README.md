# OrionOS

A minimal x86 hobbyist operating system demonstrating real-mode to protected-mode transition with a freestanding C kernel.

---

## Features

- **Custom Bootloader**: 16-bit MBR-compliant assembly bootloader (512 bytes)
- **GDT Setup**: Global Descriptor Table implementation for memory segmentation
- **Protected Mode**: CPU mode transition from 16-bit to 32-bit
- **C Kernel**: Freestanding C environment with direct VGA memory access
- **Automated Build**: Makefile-driven compilation and linking workflow

---

## Prerequisites

Requires a Linux system with 32-bit toolchain support.

### Fedora
```bash
sudo dnf install nasm gcc glibc-devel.i686 qemu-system-x86
```

### Ubuntu/Debian
```bash
sudo apt install nasm gcc-multilib build-essential qemu-system-x86
```

---

## Project Structure

| File | Description |
|------|-------------|
| `boot.asm` | Primary bootloader (BIOS interrupts, disk I/O, mode switch) |
| `gdt.asm` | Global Descriptor Table definitions (code/data segments) |
| `entry.asm` | Assembly wrapper for C `main()` calling convention |
| `kernel.c` | Kernel logic written in freestanding C |
| `Makefile` | Build automation for image generation |

---

## Build & Run

### 1. Build OS Image
Compiles all sources into bootable `os-image.bin`:
```bash
make
```

### 2. Run in QEMU
Launches the system in emulated hardware:
```bash
make run
```

### 3. Clean Artifacts
Remove generated files:
```bash
make clean
```

---

## Technical Details

### Boot Process
1. **BIOS Load**: `boot.bin` loaded at physical address `0x7C00`
2. **Kernel Load**: Bootloader reads kernel from disk to `0x1000`
3. **Mode Transition**: 
   - GDT is loaded into GDTR
   - Protection bit set in CR0
   - Far jump to 32-bit code segment
4. **Execution**: `entry.asm` invokes `kernel.c:main()`
5. **Output**: Direct character writes to VGA buffer at `0xB8000`

### Memory Layout
- `0x00000-0x7FFFF`: Low memory (bootloader, BIOS data)
- `0x07C00-0x07DFF`: Bootloader execution
- `0x10000+`: Kernel image

---

## License

Open-source. Educational use only.
