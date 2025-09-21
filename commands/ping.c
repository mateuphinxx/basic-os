#include "../drivers/vga.h"

static int ping_counter = 1;

static void fast_number_print(int number) {
    if (number == 0) {
        vga_putchar('0');
        return;
    }
    
    char buffer[12];
    int position = 0;
    
    while (number > 0) {
        buffer[position++] = '0' + (number % 10);
        number /= 10;
    }
    
    for (int index = position - 1; index >= 0; index--) {
        vga_putchar(buffer[index]);
    }
}

void cmd_ping(const char* args) {
    char target_host[32] = {0};
    int packet_count = 3;
    int character_index = 0;
    
    while (*args == ' ') args++;
    
    if (*args == '\0') {
        vga_print("Usage: ping <hostname>\n");
        vga_print("Example: ping google.com\n");
        return;
    }
    
    while (*args && *args != ' ' && character_index < 31) {
        target_host[character_index++] = *args++;
    }
    target_host[character_index] = '\0';
    
    if (target_host[0] == '\0') {
        vga_print("ping: hostname required\n");
        return;
    }
    
    vga_print("PING ");
    vga_print(target_host);
    vga_print(" (192.168.1.");
    fast_number_print(ping_counter + 100);
    vga_print("): 56 bytes of data\n");
    
    for (int packet_sequence = 1; packet_sequence <= packet_count; packet_sequence++) {
        vga_print("64 bytes from 192.168.1.");
        fast_number_print(ping_counter + 100);
        vga_print(": icmp_seq=");
        fast_number_print(packet_sequence);
        vga_print(" time=");
        fast_number_print(15 + (ping_counter * packet_sequence) % 30);
        vga_print(" ms\n");
    }
    
    vga_print("\n--- ");
    vga_print(target_host);
    vga_print(" ping statistics ---\n");
    fast_number_print(packet_count);
    vga_print(" packets transmitted, ");
    fast_number_print(packet_count);
    vga_print(" received, 0% packet loss\n");
    
    ping_counter = (ping_counter + 1) % 50;
}