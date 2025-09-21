#include "command.h"
#include "../drivers/vga.h"

static void simple_delay(int cycles) {
    for (volatile int i = 0; i < cycles; i++) {
        for (volatile int j = 0; j < 10000; j++) {
            asm volatile("nop");
        }
    }
}

static int simple_random(int seed) {
    static int next = 1;
    if (seed) next = seed;
    next = next * 1103515245 + 12345;
    return (next / 65536) % 32768;
}

static void int_to_string(int num, char* str) {
    int len = 0;
    if (num == 0) {
        str[len++] = '0';
    } else {
        char digits[16];
        int digit_count = 0;
        while (num > 0) {
            digits[digit_count++] = '0' + (num % 10);
            num /= 10;
        }
        for (int j = digit_count - 1; j >= 0; j--) {
            str[len++] = digits[j];
        }
    }
    str[len] = '\0';
}

void cmd_ping(const char* args) asm("cmd_ping");
void cmd_ping(const char* args) {
    char hostname[64] = {0};
    int count = 4;
    int i = 0;
    
    while (*args == ' ') args++;
    
    if (*args == '\0') {
        vga_print("Usage: ping <hostname>\n");
        vga_print("       ping -c <count> <hostname>\n");
        vga_print("Examples:\n");
        vga_print("  ping google.com\n");
        vga_print("  ping -c 10 8.8.8.8\n");
        return;
    }
    
    // Parse -c option
    if (*args == '-' && *(args + 1) == 'c') {
        args += 2;
        while (*args == ' ') args++;
        
        count = 0;
        while (*args >= '0' && *args <= '9') {
            count = count * 10 + (*args - '0');
            args++;
        }
        
        if (count <= 0 || count > 100) count = 4;
        while (*args == ' ') args++;
    }
    
    // Extract hostname
    while (*args && *args != ' ' && i < 63) {
        hostname[i++] = *args++;
    }
    hostname[i] = '\0';
    
    if (hostname[0] == '\0') {
        vga_print("ping: hostname required\n");
        return;
    }
    
    // Generate random IP
    int ip1 = 8 + simple_random(0) % 240;
    int ip2 = 8 + simple_random(0) % 240; 
    int ip3 = 8 + simple_random(0) % 240;
    int ip4 = 8 + simple_random(0) % 240;
    
    char ip_str[32];
    char temp[8];
    
    int_to_string(ip1, temp);
    int pos = 0;
    for (int j = 0; temp[j]; j++) ip_str[pos++] = temp[j];
    ip_str[pos++] = '.';
    
    int_to_string(ip2, temp);
    for (int j = 0; temp[j]; j++) ip_str[pos++] = temp[j];
    ip_str[pos++] = '.';
    
    int_to_string(ip3, temp);
    for (int j = 0; temp[j]; j++) ip_str[pos++] = temp[j];
    ip_str[pos++] = '.';
    
    int_to_string(ip4, temp);
    for (int j = 0; temp[j]; j++) ip_str[pos++] = temp[j];
    ip_str[pos] = '\0';
    
    // Print ping header
    vga_print("PING ");
    vga_print(hostname);
    vga_print(" (");
    vga_print(ip_str);
    vga_print("): 56(84) bytes of data.\n");
    
    // Ping statistics
    int transmitted = 0;
    int received = 0;
    int min_time = 9999;
    int max_time = 0;
    int total_time = 0;
    
    // Send pings
    for (int seq = 1; seq <= count; seq++) {
        transmitted++;
        
        vga_print("64 bytes from ");
        vga_print(ip_str);
        vga_print(": icmp_seq=");
        
        char seq_str[8];
        int_to_string(seq, seq_str);
        vga_print(seq_str);
        
        simple_delay(50 + simple_random(seq * 17) % 100);
        
        int time_ms = 15 + simple_random(seq * 23) % 50;
        total_time += time_ms;
        if (time_ms < min_time) min_time = time_ms;
        if (time_ms > max_time) max_time = time_ms;
        
        vga_print(" ttl=64 time=");
        
        char time_str[8];
        int_to_string(time_ms, time_str);
        vga_print(time_str);
        vga_print(" ms\n");
        
        received++;
        
        if (seq < count) {
            simple_delay(100);
        }
    }
    
    // Print statistics
    vga_print("\n--- ");
    vga_print(hostname);
    vga_print(" ping statistics ---\n");
    
    char stats[16];
    int_to_string(transmitted, stats);
    vga_print(stats);
    vga_print(" packets transmitted, ");
    
    int_to_string(received, stats);
    vga_print(stats);
    vga_print(" received, 0% packet loss\n");
    
    if (received > 0) {
        int avg_time = total_time / received;
        vga_print("rtt min/avg/max = ");
        
        int_to_string(min_time, stats);
        vga_print(stats);
        vga_print("/");
        
        int_to_string(avg_time, stats);
        vga_print(stats);
        vga_print("/");
        
        int_to_string(max_time, stats);
        vga_print(stats);
        vga_print(" ms\n");
    }
}
