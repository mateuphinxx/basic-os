#include "command.h"
#include "../drivers/vga.h"

static vga_color determine_color_value(const char* color_identifier) {
    const char* identifier = color_identifier;
    
    if (identifier[0] == 'b' && identifier[1] == 'l' && identifier[2] == 'a' && identifier[3] == 'c' && identifier[4] == 'k' && identifier[5] == '\0') return VGA_COLOR_BLACK;
    if (identifier[0] == 'b' && identifier[1] == 'l' && identifier[2] == 'u' && identifier[3] == 'e' && identifier[4] == '\0') return VGA_COLOR_BLUE;
    if (identifier[0] == 'g' && identifier[1] == 'r' && identifier[2] == 'e' && identifier[3] == 'e' && identifier[4] == 'n' && identifier[5] == '\0') return VGA_COLOR_GREEN;
    if (identifier[0] == 'c' && identifier[1] == 'y' && identifier[2] == 'a' && identifier[3] == 'n' && identifier[4] == '\0') return VGA_COLOR_CYAN;
    if (identifier[0] == 'r' && identifier[1] == 'e' && identifier[2] == 'd' && identifier[3] == '\0') return VGA_COLOR_RED;
    if (identifier[0] == 'm' && identifier[1] == 'a' && identifier[2] == 'g' && identifier[3] == 'e' && identifier[4] == 'n' && identifier[5] == 't' && identifier[6] == 'a' && identifier[7] == '\0') return VGA_COLOR_MAGENTA;
    if (identifier[0] == 'b' && identifier[1] == 'r' && identifier[2] == 'o' && identifier[3] == 'w' && identifier[4] == 'n' && identifier[5] == '\0') return VGA_COLOR_BROWN;
    if (identifier[0] == 'l' && identifier[1] == 'i' && identifier[2] == 'g' && identifier[3] == 'h' && identifier[4] == 't' && identifier[5] == 'g' && identifier[6] == 'r' && identifier[7] == 'a' && identifier[8] == 'y' && identifier[9] == '\0') return VGA_COLOR_LIGHT_GREY;
    if (identifier[0] == 'd' && identifier[1] == 'a' && identifier[2] == 'r' && identifier[3] == 'k' && identifier[4] == 'g' && identifier[5] == 'r' && identifier[6] == 'a' && identifier[7] == 'y' && identifier[8] == '\0') return VGA_COLOR_DARK_GREY;
    if (identifier[0] == 'l' && identifier[1] == 'i' && identifier[2] == 'g' && identifier[3] == 'h' && identifier[4] == 't' && identifier[5] == 'b' && identifier[6] == 'l' && identifier[7] == 'u' && identifier[8] == 'e' && identifier[9] == '\0') return VGA_COLOR_LIGHT_BLUE;
    if (identifier[0] == 'l' && identifier[1] == 'i' && identifier[2] == 'g' && identifier[3] == 'h' && identifier[4] == 't' && identifier[5] == 'g' && identifier[6] == 'r' && identifier[7] == 'e' && identifier[8] == 'e' && identifier[9] == 'n' && identifier[10] == '\0') return VGA_COLOR_LIGHT_GREEN;
    if (identifier[0] == 'l' && identifier[1] == 'i' && identifier[2] == 'g' && identifier[3] == 'h' && identifier[4] == 't' && identifier[5] == 'c' && identifier[6] == 'y' && identifier[7] == 'a' && identifier[8] == 'n' && identifier[9] == '\0') return VGA_COLOR_LIGHT_CYAN;
    if (identifier[0] == 'l' && identifier[1] == 'i' && identifier[2] == 'g' && identifier[3] == 'h' && identifier[4] == 't' && identifier[5] == 'r' && identifier[6] == 'e' && identifier[7] == 'd' && identifier[8] == '\0') return VGA_COLOR_LIGHT_RED;
    if (identifier[0] == 'l' && identifier[1] == 'i' && identifier[2] == 'g' && identifier[3] == 'h' && identifier[4] == 't' && identifier[5] == 'm' && identifier[6] == 'a' && identifier[7] == 'g' && identifier[8] == 'e' && identifier[9] == 'n' && identifier[10] == 't' && identifier[11] == 'a' && identifier[12] == '\0') return VGA_COLOR_LIGHT_MAGENTA;
    if (identifier[0] == 'y' && identifier[1] == 'e' && identifier[2] == 'l' && identifier[3] == 'l' && identifier[4] == 'o' && identifier[5] == 'w' && identifier[6] == '\0') return VGA_COLOR_LIGHT_BROWN;
    if (identifier[0] == 'w' && identifier[1] == 'h' && identifier[2] == 'i' && identifier[3] == 't' && identifier[4] == 'e' && identifier[5] == '\0') return VGA_COLOR_WHITE;
    
    return VGA_COLOR_WHITE;
}

void cmd_color(const char* args) asm("cmd_color");
void cmd_color(const char* args) {
    char requested_color[20] = {0};
    
    int character_position = 0;
    while (*args == ' ') args++;
    
    while (*args && *args != ' ' && character_position < 19) {
        requested_color[character_position++] = *args++;
    }
    requested_color[character_position] = '\0';
    
    if (requested_color[0] == '\0') {
        vga_print("Usage: color <color>\n");
        vga_print("Example: color green\n");
        vga_print("Type 'colors' to see available colors.\n");
        return;
    }
    
    vga_color selected_foreground = determine_color_value(requested_color);
    vga_set_color(selected_foreground, VGA_COLOR_BLUE);
    
    vga_print("Text color changed to ");
    vga_print(requested_color);
    vga_print("\n");
}

void cmd_colors(const char* args) asm("cmd_colors");
void cmd_colors(const char* args) {
    (void)args;
    
    vga_print_colored("Available colors:\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
    vga_print("Basic colors: ");
    vga_print_colored("black ", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    vga_print_colored("blue ", VGA_COLOR_BLUE, VGA_COLOR_WHITE);
    vga_print_colored("green ", VGA_COLOR_GREEN, VGA_COLOR_WHITE);
    vga_print_colored("cyan ", VGA_COLOR_CYAN, VGA_COLOR_WHITE);
    vga_print_colored("red ", VGA_COLOR_RED, VGA_COLOR_WHITE);
    vga_print_colored("magenta ", VGA_COLOR_MAGENTA, VGA_COLOR_WHITE);
    vga_print_colored("brown ", VGA_COLOR_BROWN, VGA_COLOR_WHITE);
    vga_print_colored("lightgray", VGA_COLOR_LIGHT_GREY, VGA_COLOR_WHITE);
    vga_print("\nBright colors: ");
    vga_print_colored("darkgray ", VGA_COLOR_DARK_GREY, VGA_COLOR_WHITE);
    vga_print_colored("lightblue ", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
    vga_print_colored("lightgreen ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_WHITE);
    vga_print_colored("lightcyan ", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_WHITE);
    vga_print_colored("lightred ", VGA_COLOR_LIGHT_RED, VGA_COLOR_WHITE);
    vga_print_colored("lightmagenta ", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_WHITE);
    vga_print_colored("yellow ", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_WHITE);
    vga_print_colored("white", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_print("\n");
}
