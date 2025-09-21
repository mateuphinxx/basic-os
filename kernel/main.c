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
    vga_print("BasicOS v1.0 - Kernel Started!\n");
    vga_print("Protected mode active...\n\n");
    
    vga_print("Initializing IDT...\n");
    idt_init();
    vga_print("IDT initialized.\n");
    
    vga_print("Initializing Memory...\n");
    memory_init();
    vga_print("Memory initialized.\n");
    
    vga_print("Initializing Keyboard...\n");
    keyboard_init();
    vga_print("Keyboard initialized.\n\n");
    
    vga_print("BasicOS Ready!\n");
    vga_print("Starting shell...\n\n");
    
    shell_init();
    shell_run();
    
    while(1) {
        asm volatile("hlt");
    }
}
