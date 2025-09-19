#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../interrupts/idt.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_init(void) asm("keyboard_init");
void keyboard_handler(registers_t regs) asm("keyboard_handler");
char keyboard_getchar(void) asm("keyboard_getchar");
int keyboard_available(void) asm("keyboard_available");

#endif
