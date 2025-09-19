#include "vga.h"

static volatile unsigned short* vga_buffer = (unsigned short*) VGA_MEMORY;
static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = 0x07;

static unsigned char vga_entry_color(vga_color fg, vga_color bg) {
    return fg | bg << 4;
}

static unsigned short vga_entry(unsigned char uc, unsigned char color) {
    return (unsigned short) uc | (unsigned short) color << 8;
}

void vga_init(void) asm("vga_init");
void vga_init(void) {
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_clear();
}

void vga_clear(void) asm("vga_clear");
void vga_clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', current_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_set_color(vga_color fg, vga_color bg) asm("vga_set_color");
void vga_set_color(vga_color fg, vga_color bg) {
    current_color = vga_entry_color(fg, bg);
}

void vga_putchar(char c) asm("vga_putchar");
void vga_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        const int index = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[index] = vga_entry(c, current_color);
        cursor_x++;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        for (int y = 1; y < VGA_HEIGHT; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[(y-1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
            }
        }
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(VGA_HEIGHT-1) * VGA_WIDTH + x] = vga_entry(' ', current_color);
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void vga_print(const char* str) asm("vga_print");
void vga_print(const char* str) {
    while (*str) {
        vga_putchar(*str);
        str++;
    }
}

void vga_set_cursor(int x, int y) asm("vga_set_cursor");
void vga_set_cursor(int x, int y) {
    cursor_x = x;
    cursor_y = y;
}
