CC = gcc
ASM = nasm
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -Wno-main -fno-zero-initialized-in-bss -fno-pic -c -I.
LDFLAGS = -m elf_i386 -T link.ld
ASMFLAGS = -f elf32

BUILD_DIR = build
SOURCES_C = kernel/main.c drivers/vga.c drivers/keyboard.c memory/memory.c interrupts/idt.c shell/shell.c
SOURCES_ASM = kernel/kernel_entry.asm interrupts/interrupt.asm
OBJECTS_C = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCES_C)))
OBJECTS_ASM = $(patsubst %.asm,$(BUILD_DIR)/%.o,$(notdir $(SOURCES_ASM)))

all: $(BUILD_DIR)/basicos.bin

$(BUILD_DIR)/basicos.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/basicos.bin

$(BUILD_DIR)/boot.bin: boot/boot.asm | $(BUILD_DIR)
	$(ASM) -f bin boot/boot.asm -o $(BUILD_DIR)/boot.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	ld $(LDFLAGS) -o $(BUILD_DIR)/kernel.bin $(OBJECTS_ASM) $(OBJECTS_C)

$(BUILD_DIR)/main.o: kernel/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/vga.o: drivers/vga.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/keyboard.o: drivers/keyboard.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/memory.o: memory/memory.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/idt.o: interrupts/idt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/shell.o: shell/shell.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kernel_entry.o: kernel/kernel_entry.asm | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/interrupt.o: interrupts/interrupt.asm | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: $(BUILD_DIR)/basicos.bin
	qemu-system-i386 -fda $(BUILD_DIR)/basicos.bin -nographic

run-gui: $(BUILD_DIR)/basicos.bin
	qemu-system-i386 -fda $(BUILD_DIR)/basicos.bin

debug: $(BUILD_DIR)/basicos.bin
	qemu-system-i386 -fda $(BUILD_DIR)/basicos.bin -s -S

copy-from-docker:
	docker run --rm -v ${PWD}:/workspace basicos-dev make all
	@echo "BasicOS binary copied to: build/basicos.bin"
	@echo "You can now run it with: qemu-system-i386 -fda build/basicos.bin"

docker-build:
	docker build -t basicos-dev .

docker-shell:
	docker run --rm -it -v ${PWD}:/workspace basicos-dev bash

.PHONY: all clean run run-gui debug copy-from-docker docker-build docker-shell
