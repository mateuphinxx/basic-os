#ifndef COMMAND_H
#define COMMAND_H

typedef struct {
    const char* name;
    const char* description;
    void (*handler)(const char* args);
} command_t;

void cmd_help(const char* args) asm("cmd_help");
void cmd_clear(const char* args) asm("cmd_clear");
void cmd_echo(const char* args) asm("cmd_echo");
void cmd_mem(const char* args) asm("cmd_mem");
void cmd_color(const char* args) asm("cmd_color");
void cmd_colors(const char* args) asm("cmd_colors");
void cmd_ping(const char* args) asm("cmd_ping");
void cmd_reboot(const char* args) asm("cmd_reboot");

extern const command_t commands[];
extern const int command_count;

void execute_command(const char* input) asm("execute_command");

#endif
