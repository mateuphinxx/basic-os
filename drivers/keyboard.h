#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../interrupts/idt.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_init(void);
void keyboard_handler(registers_t regs);
char keyboard_getchar(void);
int keyboard_available(void);

#endif
