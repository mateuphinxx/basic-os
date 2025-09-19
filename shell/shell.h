#ifndef SHELL_H
#define SHELL_H

#define SHELL_BUFFER_SIZE 256

void shell_init(void) asm("shell_init");
void shell_run(void) asm("shell_run");
void shell_execute(const char* command) asm("shell_execute");

#endif
