#include "shell.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../memory/memory.h"
#include "../commands/command.h"

static char shell_buffer[SHELL_BUFFER_SIZE];
static int buffer_pos = 0;

static void shell_clear(void) {
    vga_clear();
    vga_print_colored("BasicOS Shell", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLUE);
    vga_print("\nType 'help' for commands\n\n");
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
                execute_command(shell_buffer);
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
