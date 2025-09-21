#include "shell.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../memory/memory.h"

static char shell_buffer[SHELL_BUFFER_SIZE];
static int buffer_pos = 0;

static int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

static void shell_clear(void) {
    vga_clear();
    vga_print_colored("BasicOS Shell", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLUE);
    vga_print("\nType 'help' for commands\n\n");
}

static void shell_help(void) {
    vga_print_colored("Available commands:\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
    vga_print("  help      - Show this help\n");
    vga_print("  clear     - Clear screen\n");
    vga_print("  echo      - Echo text\n");
    vga_print("  mem       - Memory info\n");
    vga_print("  color     - Set text colors (color <fg> <bg>)\n");
    vga_print("  colors    - Show available colors\n");
    vga_print("  reboot    - Restart system\n\n");
    vga_print_colored("Color examples:\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    vga_print("  color red black    - Red text on black background\n");
    vga_print("  color white blue   - White text on blue background\n");
}

static void shell_echo(const char* args) {
    if (*args == ' ') args++;
    vga_print(args);
    vga_print("\n");
}

static void shell_memory_info(void) {
    vga_print("Memory Information:\n");
    vga_print("Heap Start: 0x100000\n");
    vga_print("Heap Size:  1MB\n");
}

static vga_color parse_color(const char* color_name) {
    if (strcmp(color_name, "black") == 0) return VGA_COLOR_BLACK;
    if (strcmp(color_name, "blue") == 0) return VGA_COLOR_BLUE;
    if (strcmp(color_name, "green") == 0) return VGA_COLOR_GREEN;
    if (strcmp(color_name, "cyan") == 0) return VGA_COLOR_CYAN;
    if (strcmp(color_name, "red") == 0) return VGA_COLOR_RED;
    if (strcmp(color_name, "magenta") == 0) return VGA_COLOR_MAGENTA;
    if (strcmp(color_name, "brown") == 0) return VGA_COLOR_BROWN;
    if (strcmp(color_name, "lightgray") == 0) return VGA_COLOR_LIGHT_GREY;
    if (strcmp(color_name, "darkgray") == 0) return VGA_COLOR_DARK_GREY;
    if (strcmp(color_name, "lightblue") == 0) return VGA_COLOR_LIGHT_BLUE;
    if (strcmp(color_name, "lightgreen") == 0) return VGA_COLOR_LIGHT_GREEN;
    if (strcmp(color_name, "lightcyan") == 0) return VGA_COLOR_LIGHT_CYAN;
    if (strcmp(color_name, "lightred") == 0) return VGA_COLOR_LIGHT_RED;
    if (strcmp(color_name, "lightmagenta") == 0) return VGA_COLOR_LIGHT_MAGENTA;
    if (strcmp(color_name, "yellow") == 0) return VGA_COLOR_LIGHT_BROWN;
    if (strcmp(color_name, "white") == 0) return VGA_COLOR_WHITE;
    return VGA_COLOR_WHITE;
}

static void shell_show_colors(void) {
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

static void shell_set_color(const char* args) {
    char fg_name[20] = {0};
    char bg_name[20] = {0};
    
    int i = 0, j = 0;
    while (*args == ' ') args++;
    
    while (*args && *args != ' ' && i < 19) {
        fg_name[i++] = *args++;
    }
    fg_name[i] = '\0';
    
    while (*args == ' ') args++;
    
    while (*args && *args != ' ' && j < 19) {
        bg_name[j++] = *args++;
    }
    bg_name[j] = '\0';
    
    if (fg_name[0] == '\0') {
        vga_print("Usage: color <foreground> <background>\n");
        vga_print("Example: color green black\n");
        return;
    }
    
    vga_color fg = parse_color(fg_name);
    vga_color bg = bg_name[0] ? parse_color(bg_name) : VGA_COLOR_BLUE;
    
    vga_set_color(fg, bg);
    vga_print("Color changed to ");
    vga_print(fg_name);
    vga_print(" on ");
    vga_print(bg_name[0] ? bg_name : "blue");
    vga_print("\n");
}

static void shell_reboot(void) {
    vga_print_colored("Rebooting system...\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE);
    asm volatile("cli");
    asm volatile("outb %%al, %%dx" : : "a"(0xFE), "d"(0x64));
    while(1) asm volatile("hlt");
}

void shell_init(void) asm("shell_init");
void shell_init(void) {
    shell_clear();
}

void shell_run(void) asm("shell_run");
void shell_run(void) {
    vga_print_colored("$ ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    
    while (1) {
        char c = keyboard_getchar();
        
        if (c == '\n') {
            vga_putchar('\n');
            shell_buffer[buffer_pos] = '\0';
            
            if (buffer_pos > 0) {
                shell_execute(shell_buffer);
            }
            
            buffer_pos = 0;
            vga_print_colored("$ ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
        } else if (c == '\b' || c == 127) {
            if (buffer_pos > 0) {
                buffer_pos--;
                vga_putchar('\b');
            }
        } else if (c >= 32 && c <= 126 && buffer_pos < SHELL_BUFFER_SIZE - 1) {
            shell_buffer[buffer_pos++] = c;
            vga_putchar(c);
        }
    }
}

void shell_execute(const char* command) asm("shell_execute");
void shell_execute(const char* command) {
    if (strcmp(command, "help") == 0) {
        shell_help();
    } else if (strcmp(command, "clear") == 0) {
        shell_clear();
    } else if (command[0] == 'e' && command[1] == 'c' && command[2] == 'h' && command[3] == 'o') {
        shell_echo(command + 4);
    } else if (strcmp(command, "mem") == 0) {
        shell_memory_info();
    } else if (command[0] == 'c' && command[1] == 'o' && command[2] == 'l' && command[3] == 'o' && command[4] == 'r' && command[5] == ' ') {
        shell_set_color(command + 6);
    } else if (strcmp(command, "colors") == 0) {
        shell_show_colors();
    } else if (strcmp(command, "reboot") == 0) {
        shell_reboot();
    } else {
        vga_print_colored("Unknown command: ", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE);
        vga_print(command);
        vga_print("\n");
        vga_print("Type 'help' for available commands.\n");
    }
}
