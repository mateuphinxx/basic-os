#ifndef VGA_H
#define VGA_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

void vga_init(void) asm("vga_init");
void vga_clear(void) asm("vga_clear");
void vga_putchar(char c) asm("vga_putchar");
void vga_print(const char* str) asm("vga_print");
void vga_print_colored(const char* str, vga_color fg, vga_color bg) asm("vga_print_colored");
void vga_set_color(vga_color fg, vga_color bg) asm("vga_set_color");
void vga_set_cursor(int x, int y) asm("vga_set_cursor");
void vga_get_cursor(int* x, int* y) asm("vga_get_cursor");
unsigned char vga_get_current_color(void) asm("vga_get_current_color");

#endif
