#include "../drivers/vga.h"

void cmd_clear(const char* args) {
    (void)args;
    
    vga_clear();
    vga_print_colored("BasicOS Shell", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLUE);
    vga_print("\nType 'help' for commands\n\n");
}

void cmd_reboot(const char* args) {
    (void)args;
    
    vga_print_colored("Rebooting system...\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE);
    asm volatile("cli");
    asm volatile("outb %%al, %%dx" : : "a"(0xFE), "d"(0x64));
    while(1) asm volatile("hlt");
}
