COMPILER = gcc
ASSEMBLER = nasm
LINKER = ld

COMPILER_OPTIONS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -Wno-main -fno-zero-initialized-in-bss -fno-pic -c -I.
LINKER_OPTIONS = -m elf_i386 -T link.ld -e kernel_entry --print-map
ASSEMBLER_OPTIONS = -f elf32

OUTPUT_DIRECTORY = build_output

BOOTLOADER_FILE = boot/boot.asm
KERNEL_C_FILES = kernel/main.c
DRIVER_C_FILES = drivers/vga.c drivers/keyboard.c
MEMORY_C_FILES = memory/memory.c
INTERRUPT_C_FILES = interrupts/idt.c
SHELL_C_FILES = shell/shell.c
COMMAND_C_FILES = commands/help.c commands/ping.c commands/color.c commands/system.c commands/info.c

KERNEL_ASM_FILES = kernel/kernel_entry.asm
INTERRUPT_ASM_FILES = interrupts/interrupt.asm

ALL_C_SOURCES = $(KERNEL_C_FILES) $(DRIVER_C_FILES) $(MEMORY_C_FILES) $(INTERRUPT_C_FILES) $(SHELL_C_FILES) $(COMMAND_C_FILES)
ALL_ASM_SOURCES = $(KERNEL_ASM_FILES) $(INTERRUPT_ASM_FILES)

C_OBJECT_FILES = $(OUTPUT_DIRECTORY)/main.o $(OUTPUT_DIRECTORY)/vga.o $(OUTPUT_DIRECTORY)/keyboard.o $(OUTPUT_DIRECTORY)/memory.o $(OUTPUT_DIRECTORY)/idt.o $(OUTPUT_DIRECTORY)/shell.o $(OUTPUT_DIRECTORY)/help.o $(OUTPUT_DIRECTORY)/ping.o $(OUTPUT_DIRECTORY)/color.o $(OUTPUT_DIRECTORY)/system.o $(OUTPUT_DIRECTORY)/info.o
ASM_OBJECT_FILES = $(OUTPUT_DIRECTORY)/kernel_entry.o $(OUTPUT_DIRECTORY)/interrupt.o
COMPLETE_OBJECT_SET = $(C_OBJECT_FILES) $(ASM_OBJECT_FILES)

.DEFAULT_GOAL := complete_build
.PHONY: complete_build remove_files execute_local execute_container execute_with_debug execute_with_gdb prepare_container container_shell information display_help compile_sources link_sources verify_objects check_symbols

complete_build: $(OUTPUT_DIRECTORY)/basicos.bin
	@echo "BasicOS compilation finished"
	@echo "Final image: $(OUTPUT_DIRECTORY)/basicos.bin"
	@echo "Total size: `wc -c < $(OUTPUT_DIRECTORY)/basicos.bin` bytes"
	@echo "Verifying object files..."
	@ls -la $(OUTPUT_DIRECTORY)/*.o || echo "Object files check complete"

$(OUTPUT_DIRECTORY)/basicos.bin: $(OUTPUT_DIRECTORY)/boot.bin $(OUTPUT_DIRECTORY)/kernel.bin
	@echo "Combining bootloader and kernel..."
	cat $(OUTPUT_DIRECTORY)/boot.bin $(OUTPUT_DIRECTORY)/kernel.bin > $@

$(OUTPUT_DIRECTORY)/boot.bin: boot/boot.asm | $(OUTPUT_DIRECTORY)
	@echo "Processing bootloader assembly..."
	$(ASSEMBLER) -f bin $< -o $@

$(OUTPUT_DIRECTORY)/kernel.bin: $(COMPLETE_OBJECT_SET) link.ld | $(OUTPUT_DIRECTORY)
	@echo "Creating kernel binary..."
	@echo "Linking with objects: $(COMPLETE_OBJECT_SET)"
	$(LINKER) $(LINKER_OPTIONS) -o $@ \
		$(OUTPUT_DIRECTORY)/kernel_entry.o \
		$(OUTPUT_DIRECTORY)/main.o \
		$(OUTPUT_DIRECTORY)/vga.o \
		$(OUTPUT_DIRECTORY)/keyboard.o \
		$(OUTPUT_DIRECTORY)/memory.o \
		$(OUTPUT_DIRECTORY)/idt.o \
		$(OUTPUT_DIRECTORY)/interrupt.o \
		$(OUTPUT_DIRECTORY)/shell.o \
		$(OUTPUT_DIRECTORY)/help.o \
		$(OUTPUT_DIRECTORY)/ping.o \
		$(OUTPUT_DIRECTORY)/color.o \
		$(OUTPUT_DIRECTORY)/system.o \
		$(OUTPUT_DIRECTORY)/info.o

$(OUTPUT_DIRECTORY)/main.o: kernel/main.c | $(OUTPUT_DIRECTORY)
	@echo "Processing kernel main source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/vga.o: drivers/vga.c | $(OUTPUT_DIRECTORY)
	@echo "Processing video driver source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/keyboard.o: drivers/keyboard.c | $(OUTPUT_DIRECTORY)
	@echo "Processing keyboard driver source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/memory.o: memory/memory.c | $(OUTPUT_DIRECTORY)
	@echo "Processing memory management source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/idt.o: interrupts/idt.c | $(OUTPUT_DIRECTORY)
	@echo "Processing interrupt descriptor table source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/shell.o: shell/shell.c $(OUTPUT_DIRECTORY)/help.o $(OUTPUT_DIRECTORY)/ping.o $(OUTPUT_DIRECTORY)/color.o $(OUTPUT_DIRECTORY)/system.o $(OUTPUT_DIRECTORY)/info.o | $(OUTPUT_DIRECTORY)
	@echo "Processing shell interface source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/help.o: commands/help.c | $(OUTPUT_DIRECTORY)
	@echo "Processing help command source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/ping.o: commands/ping.c | $(OUTPUT_DIRECTORY)
	@echo "Processing ping command source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/color.o: commands/color.c | $(OUTPUT_DIRECTORY)
	@echo "Processing color command source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/system.o: commands/system.c | $(OUTPUT_DIRECTORY)
	@echo "Processing system command source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/info.o: commands/info.c | $(OUTPUT_DIRECTORY)
	@echo "Processing info command source"
	$(COMPILER) $(COMPILER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/kernel_entry.o: kernel/kernel_entry.asm | $(OUTPUT_DIRECTORY)
	@echo "Processing kernel entry assembly"
	$(ASSEMBLER) $(ASSEMBLER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY)/interrupt.o: interrupts/interrupt.asm | $(OUTPUT_DIRECTORY)
	@echo "Processing interrupt handler assembly"
	$(ASSEMBLER) $(ASSEMBLER_OPTIONS) $< -o $@

$(OUTPUT_DIRECTORY):
	@echo "Creating output directory"
	@mkdir -p $(OUTPUT_DIRECTORY)

remove_files:
	@echo "Removing build artifacts"
	rm -rf $(OUTPUT_DIRECTORY)
	@echo "Cleanup completed"

execute_local: $(OUTPUT_DIRECTORY)/basicos.bin
	@echo "Launching BasicOS locally"
	qemu-system-x86_64 -fda $< -boot a

execute_container: $(OUTPUT_DIRECTORY)/basicos.bin
	@echo "Launching BasicOS in container mode"
	qemu-system-x86_64 -fda $< -boot a -nographic

execute_with_debug: $(OUTPUT_DIRECTORY)/basicos.bin
	@echo "Launching BasicOS with debug interface"
	qemu-system-x86_64 -fda $< -boot a -nographic -no-reboot -monitor stdio

execute_with_gdb: $(OUTPUT_DIRECTORY)/basicos.bin
	@echo "Launching BasicOS with debugger support"
	@echo "Connect debugger: gdb -ex 'target remote localhost:1234'"
	qemu-system-x86_64 -fda $< -boot a -s -S -nographic -no-reboot

prepare_container:
	@echo "Preparing development container"
	docker build -t basicos-dev .

container_shell: prepare_container
	@echo "Starting development container shell"
	docker run --rm -it -v $(PWD):/workspace basicos-dev bash

container_execute: prepare_container $(OUTPUT_DIRECTORY)/basicos.bin
	@echo "Executing BasicOS through container"
	docker run --rm -it -v $(PWD):/workspace basicos-dev make execute_container

compile_sources: $(COMPLETE_OBJECT_SET)
	@echo "Source compilation completed"

link_sources: $(OUTPUT_DIRECTORY)/kernel.bin
	@echo "Kernel linking completed"

information:
	@echo "BasicOS build configuration"
	@echo "Compiler: $(COMPILER)"
	@echo "Assembler: $(ASSEMBLER)"
	@echo "Linker: $(LINKER)"
	@echo "C sources: $(ALL_C_SOURCES)"
	@echo "Assembly sources: $(ALL_ASM_SOURCES)"
	@echo "C objects: $(C_OBJECT_FILES)"
	@echo "Assembly objects: $(ASM_OBJECT_FILES)"
	@echo "Output directory: $(OUTPUT_DIRECTORY)"

verify_objects: $(COMPLETE_OBJECT_SET)
	@echo "Verifying all object files exist..."
	@for obj in $(COMPLETE_OBJECT_SET); do \
		if [ -f "$$obj" ]; then \
			echo "✓ $$obj exists"; \
		else \
			echo "✗ $$obj MISSING"; \
		fi; \
	done
	@echo "Object verification complete"

check_symbols: $(OUTPUT_DIRECTORY)/kernel.bin
	@echo "Checking symbols in kernel binary..."
	@objdump -t $(OUTPUT_DIRECTORY)/kernel.bin | grep -E "(cmd_|shell_)" || echo "No command symbols found"

display_help:
	@echo "BasicOS development commands"
	@echo "complete_build     - Create complete OS image"
	@echo "remove_files       - Clean all build artifacts"
	@echo "execute_local      - Run OS with graphical interface"
	@echo "execute_container  - Run OS in headless mode"
	@echo "execute_with_debug - Run OS with debug output"
	@echo "execute_with_gdb   - Run OS with debugger"
	@echo "prepare_container  - Setup development container"
	@echo "container_shell    - Open container development shell"
	@echo "container_execute  - Run OS through container"
	@echo "compile_sources    - Compile without linking"
	@echo "link_sources       - Link without compilation"
	@echo "information        - Display configuration"
	@echo "display_help       - Show available commands"

all: complete_build
clean: remove_files
run: execute_local
run-docker: execute_container
run-debug: execute_with_debug
debug-gdb: execute_with_gdb
docker-build: prepare_container
docker-shell: container_shell
docker-run: container_execute
info: information
help: display_help
verify: verify_objects
symbols: check_symbols