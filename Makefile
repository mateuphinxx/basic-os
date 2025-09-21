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

.PHONY: all clean run run-docker run-debug run-gui debug

all: $(BUILD_DIR)/basicos.bin

$(BUILD_DIR)/basicos.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/basicos.bin

$(BUILD_DIR)/boot.bin: boot/boot.asm | $(BUILD_DIR)
	@echo "Building bootloader..."
	$(ASM) -f bin boot/boot.asm -o $@

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C) | $(BUILD_DIR)
	@echo "Linking kernel..."
	ld $(LDFLAGS) -o $@ $(OBJECTS_ASM) $(OBJECTS_C)

$(BUILD_DIR)/%.o: kernel/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: drivers/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: memory/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: interrupts/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: shell/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: kernel/%.asm | $(BUILD_DIR)
	@echo "Assembling $<..."
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/%.o: interrupts/%.asm | $(BUILD_DIR)
	@echo "Assembling $<..."
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: $(BUILD_DIR)/basicos.bin
	@echo "Starting BasicOS..."
	qemu-system-i386 -fda $(BUILD_DIR)/basicos.bin -boot a

run-docker: $(BUILD_DIR)/basicos.bin
	@echo "Starting BasicOS in Docker..."
	@echo "File size: `wc -c < $(BUILD_DIR)/basicos.bin` bytes"
	@cp $(BUILD_DIR)/basicos.bin /tmp/basicos_temp.bin
	@echo "Starting QEMU..."
	qemu-system-i386 -drive file=/tmp/basicos_temp.bin,format=raw,if=floppy,readonly=on -boot a -nographic -no-reboot

run-debug: $(BUILD_DIR)/basicos.bin
	@echo "Starting BasicOS with debug info..."
	qemu-system-i386 -drive file=$(BUILD_DIR)/basicos.bin,format=raw,if=floppy -boot a -nographic -monitor stdio -no-reboot

debug-gdb: $(BUILD_DIR)/basicos.bin
	@echo "Starting BasicOS with GDB support (run 'gdb' and 'target remote localhost:1234')..."
	qemu-system-i386 -drive file=$(BUILD_DIR)/basicos.bin,format=raw,if=floppy -boot a -s -S -nographic -no-reboot

docker-build:
	@echo "Building Docker image..."
	docker build -t basicos-dev .

docker-shell:
	@echo "Starting Docker shell..."
	docker run --rm -it -v ${PWD}:/workspace basicos-dev bash
