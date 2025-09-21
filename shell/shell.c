#include "shell.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../memory/memory.h"

extern void cmd_help(const char* args);
extern void cmd_clear(const char* args);
extern void cmd_echo(const char* args);
extern void cmd_mem(const char* args);
extern void cmd_color(const char* args);
extern void cmd_colors(const char* args);
extern void cmd_ping(const char* args);
extern void cmd_reboot(const char* args);

static char shell_buffer[SHELL_BUFFER_SIZE];
static int buffer_pos = 0;

static void shell_clear(void) {
    vga_clear();
    vga_print_colored("BasicOS Shell", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLUE);
    vga_print("\nType 'help' for commands\n\n");
}

static int string_match(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

static void process_command(const char* input) {
    char command_name[64] = {0};
    char arguments[256] = {0};
    int name_pos = 0;
    int arg_pos = 0;
    
    while (*input == ' ') input++;
    
    while (*input && *input != ' ' && name_pos < 63) {
        command_name[name_pos++] = *input++;
    }
    command_name[name_pos] = '\0';
    
    while (*input == ' ') input++;
    
    while (*input && arg_pos < 255) {
        arguments[arg_pos++] = *input++;
    }
    arguments[arg_pos] = '\0';
    
    if (string_match(command_name, "help")) {
        cmd_help(arguments);
    } else if (string_match(command_name, "clear")) {
        cmd_clear(arguments);
    } else if (string_match(command_name, "echo")) {
        cmd_echo(arguments);
    } else if (string_match(command_name, "mem")) {
        cmd_mem(arguments);
    } else if (string_match(command_name, "color")) {
        cmd_color(arguments);
    } else if (string_match(command_name, "colors")) {
        cmd_colors(arguments);
    } else if (string_match(command_name, "ping")) {
        cmd_ping(arguments);
    } else if (string_match(command_name, "reboot")) {
        cmd_reboot(arguments);
    } else {
        vga_print_colored("Unknown command: ", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE);
        vga_print(command_name);
        vga_print("\nType 'help' for available commands.\n");
    }
}

void shell_init(void) {
    shell_clear();
}

void shell_run(void) {
    vga_print_colored("$ ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    
    while (1) {
        char c = keyboard_getchar();
        
        if (c == '\n') {
            vga_putchar('\n');
            shell_buffer[buffer_pos] = '\0';
            
            if (buffer_pos > 0) {
                process_command(shell_buffer);
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