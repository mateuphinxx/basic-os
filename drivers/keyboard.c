#include "keyboard.h"

extern void register_interrupt_handler(unsigned char n, isr_t handler);

static char keyboard_buffer[256];
static int buffer_start = 0;
static int buffer_end = 0;

static unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned char keyboard_map_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int shift_pressed = 0;

static unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile("inb %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void keyboard_init(void) {
    register_interrupt_handler(33, keyboard_handler);
}

void keyboard_handler(registers_t regs) {
    (void)regs;
    
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    
    if (scancode & 0x80) {
        scancode &= 0x7F;
        if (scancode == 42 || scancode == 54) {
            shift_pressed = 0;
        }
        return;
    }
    
    if (scancode == 42 || scancode == 54) {
        shift_pressed = 1;
        return;
    }
    
    char key;
    if (shift_pressed) {
        key = keyboard_map_shift[scancode];
    } else {
        key = keyboard_map[scancode];
    }
    
    if (key) {
        keyboard_buffer[buffer_end] = key;
        buffer_end = (buffer_end + 1) % 256;
    }
}

char keyboard_getchar(void) {
    while (buffer_start == buffer_end) {
        asm volatile("sti; hlt; cli");
    }
    
    char c = keyboard_buffer[buffer_start];
    buffer_start = (buffer_start + 1) % 256;
    return c;
}

int keyboard_available(void) {
    return buffer_start != buffer_end;
}
