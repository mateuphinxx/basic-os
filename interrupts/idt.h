#ifndef IDT_H
#define IDT_H

#define IDT_ENTRIES 256

typedef struct {
    unsigned short base_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_ptr_t;

typedef struct {
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*isr_t)(registers_t);

void idt_init(void);
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void register_interrupt_handler(unsigned char n, isr_t handler);

#endif
