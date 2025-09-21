#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "memory/memory.h"
#include "interrupts/idt.h"
#include "shell/shell.h"

void __main(void) {}

extern void idt_init(void) asm("idt_init");

void main() asm("main");
void main() {
    volatile unsigned short* vga = (unsigned short*)0xB8000;
    
    vga[0] = 0x4F4D;
    vga[1] = 0x4F41;
    vga[2] = 0x4F49;
    vga[3] = 0x4F4E;
    
    vga_init();
    vga_clear();
    vga_print_colored("BasicOS v1.0 - Kernel Started!\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLUE);
    vga_print_colored("Protected mode active...\n\n", VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    
    vga_print_colored("Initializing IDT...\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
    idt_init();
    vga_print_colored("IDT initialized.\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    
    vga_print_colored("Initializing Memory...\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
    memory_init();
    vga_print_colored("Memory initialized.\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    
    vga_print_colored("Initializing Keyboard...\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
    keyboard_init();
    vga_print_colored("Keyboard initialized.\n\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    
    vga_print_colored("BasicOS Ready!\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLUE);
    vga_print_colored("Starting shell...\n\n", VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    
    shell_init();
    shell_run();
    
    while(1) {
        asm volatile("hlt");
    }
}
