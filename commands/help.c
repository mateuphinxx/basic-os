#include "command.h"
#include "../drivers/vga.h"

void cmd_help(const char* args) asm("cmd_help");
void cmd_help(const char* args) {
    (void)args;
    
    vga_print_colored("Available commands:\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
    
    for (int command_index = 0; command_index < command_count; command_index++) {
        vga_print("  ");
        vga_print_colored(commands[command_index].name, VGA_COLOR_WHITE, VGA_COLOR_BLUE);
        
        int current_name_length = 0;
        const char* current_name = commands[command_index].name;
        while (*current_name++) current_name_length++;
        
        for (int padding = current_name_length; padding < 10; padding++) {
            vga_print(" ");
        }
        
        vga_print("- ");
        vga_print(commands[command_index].description);
        vga_print("\n");
    }
}
