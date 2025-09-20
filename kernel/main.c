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
    
    vga[10] = 0x0744;
    vga[11] = 0x0744;
    
    vga_init();
    vga_print("Protected mode entered!\n");
    vga_print("Initializing systems...\n");
    
    vga[12] = 0x0745;
    vga[13] = 0x0745;
    
    idt_init();
    memory_init();
    keyboard_init();
    
    vga[14] = 0x0746;
    vga[15] = 0x0746;
    
    vga_print("BasicOS v1.0\n");
    vga_print("Welcome to BasicOS!\n\n");
    
    vga[16] = 0x0747;
    vga[17] = 0x0747;
    
    shell_init();
    shell_run();
    
    while(1) {
        asm volatile("hlt");
    }
}
