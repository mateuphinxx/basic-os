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
    vga_print("BasicOS Shell\n");
    vga_print("Type 'help' for commands\n\n");
}

static void shell_help(void) {
    vga_print("Available commands:\n");
    vga_print("  help    - Show this help\n");
    vga_print("  clear   - Clear screen\n");
    vga_print("  echo    - Echo text\n");
    vga_print("  mem     - Memory info\n");
    vga_print("  reboot  - Restart system\n");
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

static void shell_reboot(void) {
    vga_print("Rebooting system...\n");
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
    vga_print("$ ");
    
    while (1) {
        char c = keyboard_getchar();
        
        if (c == '\n') {
            vga_putchar('\n');
            shell_buffer[buffer_pos] = '\0';
            
            if (buffer_pos > 0) {
                shell_execute(shell_buffer);
            }
            
            buffer_pos = 0;
            vga_print("$ ");
        } else if (c == '\b') {
            if (buffer_pos > 0) {
                buffer_pos--;
                vga_print("\b \b");
            }
        } else if (buffer_pos < SHELL_BUFFER_SIZE - 1) {
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
    } else if (strcmp(command, "reboot") == 0) {
        shell_reboot();
    } else {
        vga_print("Unknown command: ");
        vga_print(command);
        vga_print("\n");
    }
}
