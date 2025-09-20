CC = gcc
ASM = nasm
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -Wno-main -fno-zero-initialized-in-bss -fno-pic -c -I.
LDFLAGS = -m elf_i386 -T link.ld
ASMFLAGS = -f elf32

BUILD_DIR = build_output
SOURCES_C = kernel/main.c drivers/vga.c drivers/keyboard.c memory/memory.c interrupts/idt.c shell/shell.c
SOURCES_ASM = kernel/kernel_entry.asm interrupts/interrupt.asm
OBJECTS_C = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCES_C)))
OBJECTS_ASM = $(patsubst %.asm,$(BUILD_DIR)/%.o,$(notdir $(SOURCES_ASM)))

all: $(BUILD_DIR)/basicos.bin

$(BUILD_DIR)/basicos.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/basicos.bin

$(BUILD_DIR)/boot.bin: boot/boot.asm
	@echo "Debug: checking what 'build' is..."
	@ls -la $(BUILD_DIR) 2>/dev/null || echo "No build entry visible"
	@file $(BUILD_DIR) 2>/dev/null || echo "Cannot determine build type"
	@test -f $(BUILD_DIR) && echo "build is a file" || echo "build is not a file"
	@test -d $(BUILD_DIR) && echo "build is a directory" || echo "build is not a directory"
	@echo "Attempting to remove any existing build..."
	@rm -rf $(BUILD_DIR) 2>/dev/null || echo "Cannot remove build"
	@echo "Creating build directory: $(BUILD_DIR)"
	@mkdir -p $(BUILD_DIR)
	@echo "Building bootloader..."
	$(ASM) -f bin boot/boot.asm -o $(BUILD_DIR)/boot.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	ld $(LDFLAGS) -o $(BUILD_DIR)/kernel.bin $(OBJECTS_ASM) $(OBJECTS_C)

$(BUILD_DIR)/main.o: kernel/main.c
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/vga.o: drivers/vga.c
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/keyboard.o: drivers/keyboard.c
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/memory.o: memory/memory.c
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/idt.o: interrupts/idt.c
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/shell.o: shell/shell.c
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kernel_entry.o: kernel/kernel_entry.asm
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/interrupt.o: interrupts/interrupt.asm
	@if [ -f $(BUILD_DIR) ]; then rm -f $(BUILD_DIR); fi; mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(BUILD_DIR)/basicos.bin
	qemu-system-i386 -drive format=raw,file=$(BUILD_DIR)/basicos.bin,index=0,if=floppy,readonly=on -boot a -nographic -no-reboot -no-hpet -monitor none

run-gui: $(BUILD_DIR)/basicos.bin
	qemu-system-i386 -fda $(BUILD_DIR)/basicos.bin

debug: $(BUILD_DIR)/basicos.bin
	qemu-system-i386 -fda $(BUILD_DIR)/basicos.bin -s -S

run-force: $(BUILD_DIR)/basicos.bin
	qemu-system-i386 -fda $(BUILD_DIR)/basicos.bin -boot a -nographic -no-reboot -machine pc,accel=tcg -no-hpet -rtc base=localtime

run-minimal: $(BUILD_DIR)/basicos.bin
	qemu-system-i386 -drive if=floppy,format=raw,file=$(BUILD_DIR)/basicos.bin -nographic -no-reboot

debug-build:
	@echo "Checking build directory..."
	@ls -la $(BUILD_DIR) 2>/dev/null || echo "Build directory does not exist"
	@file $(BUILD_DIR) 2>/dev/null || echo "Cannot check build directory type"

copy-from-docker:
	docker run --rm -v ${PWD}:/workspace basicos-dev make all
	@echo "BasicOS binary copied to: build/basicos.bin"
	@echo "You can now run it with: qemu-system-i386 -fda build/basicos.bin"

docker-build:
	docker build -t basicos-dev .

docker-shell:
	docker run --rm -it -v ${PWD}:/workspace basicos-dev bash

.PHONY: all clean run run-gui debug debug-build copy-from-docker docker-build docker-shell
