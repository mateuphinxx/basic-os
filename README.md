# BasicOS

A simple operating system built with Assembly and C for educational purposes.

## Features

- Custom bootloader
- 32-bit protected mode
- Memory management with heap allocation
- Interrupt handling (IDT)
- VGA text mode display driver
- PS/2 keyboard driver
- Simple shell interface

## Building

Requirements:
- GCC cross-compiler (i686-elf-gcc recommended)
- NASM assembler
- GNU Make
- QEMU (for testing)

```bash
make all
```

## Running

```bash
make run
```

## Shell Commands

- `help` - Show available commands
- `clear` - Clear screen
- `echo [text]` - Display text
- `mem` - Show memory information
- `reboot` - Restart system

## Architecture

```
boot/           - Bootloader (16-bit real mode)
kernel/         - Kernel entry and setup
drivers/        - Hardware drivers (VGA, keyboard)
memory/         - Memory management
interrupts/     - Interrupt handling
shell/          - Command shell
```
