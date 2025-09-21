#include "command.h"
#include "../drivers/vga.h"

const command_t commands[] = {
    {"help", "Show available commands", cmd_help},
    {"clear", "Clear screen", cmd_clear},
    {"echo", "Echo text", cmd_echo},
    {"mem", "Show memory information", cmd_mem},
    {"color", "Set text color", cmd_color},
    {"colors", "Show available colors", cmd_colors},
    {"ping", "Ping network host", cmd_ping},
    {"reboot", "Restart system", cmd_reboot}
};

const int command_count = sizeof(commands) / sizeof(commands[0]);

static int string_compare(const char* first, const char* second) {
    while (*first && *second && *first == *second) {
        first++;
        second++;
    }
    return *first - *second;
}

static void extract_command_parts(const char* input, char* command_name, char* arguments) {
    int position = 0;
    
    while (*input == ' ') input++;
    
    while (*input && *input != ' ' && position < 63) {
        command_name[position++] = *input++;
    }
    command_name[position] = '\0';
    
    while (*input == ' ') input++;
    
    position = 0;
    while (*input && position < 255) {
        arguments[position++] = *input++;
    }
    arguments[position] = '\0';
}

void execute_command(const char* input) asm("execute_command");
void execute_command(const char* input) {
    char command_buffer[64] = {0};
    char argument_buffer[256] = {0};
    
    extract_command_parts(input, command_buffer, argument_buffer);
    
    if (command_buffer[0] == '\0') {
        return;
    }
    
    for (int index = 0; index < command_count; index++) {
        if (string_compare(command_buffer, commands[index].name) == 0) {
            commands[index].handler(argument_buffer);
            return;
        }
    }
    
    vga_print_colored("Unknown command: ", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE);
    vga_print(command_buffer);
    vga_print("\nType 'help' for available commands.\n");
}
