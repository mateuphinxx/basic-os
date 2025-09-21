#include "../drivers/vga.h"

void cmd_help(const char* args) {
    (void)args;
    
    vga_print_colored("Available commands:\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
    vga_print("  help      - Show available commands\n");
    vga_print("  clear     - Clear screen\n");
    vga_print("  echo      - Echo text\n");
    vga_print("  mem       - Show memory information\n");
    vga_print("  color     - Set text color\n");
    vga_print("  colors    - Show available colors\n");
    vga_print("  ping      - Ping network host\n");
    vga_print("  reboot    - Restart system\n");
}
