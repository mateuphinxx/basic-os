#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "memory/memory.h"
#include "interrupts/idt.h"
#include "shell/shell.h"

void __main(void) {}

extern void idt_init(void) asm("idt_init");

void main() asm("main");
void main() {
    vga_init();
    idt_init();
    memory_init();
    keyboard_init();
    
    vga_print("BasicOS v1.0\n");
    vga_print("Welcome to BasicOS!\n\n");
    
    shell_init();
    shell_run();
    
    while(1) {
        asm("hlt");
    }
}
