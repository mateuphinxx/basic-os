#include "../drivers/vga.h"

void cmd_echo(const char* args) {
    if (*args == ' ') args++;
    vga_print(args);
    vga_print("\n");
}

void cmd_mem(const char* args) {
    (void)args;
    
    vga_print_colored("Memory Information:\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
    vga_print("Heap Start: 0x100000 (1MB)\n");
    vga_print("Heap Size:  1MB\n");
    vga_print("VGA Buffer: 0xB8000\n");
    vga_print("Stack:      0x90000\n");
    vga_print("Kernel:     0x8000\n");
}
