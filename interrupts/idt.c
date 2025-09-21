#include "idt.h"

extern void idt_flush(unsigned long);

static idt_entry_t idt_entries[IDT_ENTRIES];
static idt_ptr_t idt_ptr;
static isr_t interrupt_handlers[IDT_ENTRIES];

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

static void remap_irq(void) {
    asm volatile("outb %%al, %%dx" : : "d"(0x20), "a"(0x11));
    asm volatile("outb %%al, %%dx" : : "d"(0xA0), "a"(0x11));
    asm volatile("outb %%al, %%dx" : : "d"(0x21), "a"(0x20));
    asm volatile("outb %%al, %%dx" : : "d"(0xA1), "a"(0x28));
    asm volatile("outb %%al, %%dx" : : "d"(0x21), "a"(0x04));
    asm volatile("outb %%al, %%dx" : : "d"(0xA1), "a"(0x02));
    asm volatile("outb %%al, %%dx" : : "d"(0x21), "a"(0x01));
    asm volatile("outb %%al, %%dx" : : "d"(0xA1), "a"(0x01));
    asm volatile("outb %%al, %%dx" : : "d"(0x21), "a"(0x0));
    asm volatile("outb %%al, %%dx" : : "d"(0xA1), "a"(0x0));
}

void idt_init(void) {
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base = (unsigned int)&idt_entries;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_entries[i].base_low = 0;
        idt_entries[i].base_high = 0;
        idt_entries[i].selector = 0;
        idt_entries[i].zero = 0;
        idt_entries[i].flags = 0;
    }

    remap_irq();

    idt_set_gate(0, (unsigned long)isr0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned long)isr1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned long)isr2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned long)isr3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned long)isr4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned long)isr5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned long)isr6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned long)isr7, 0x08, 0x8E);
    idt_set_gate(8, (unsigned long)isr8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned long)isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned long)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned long)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned long)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned long)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned long)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned long)isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned long)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned long)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned long)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned long)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned long)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned long)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned long)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned long)isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned long)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned long)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned long)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned long)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned long)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned long)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned long)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned long)isr31, 0x08, 0x8E);

    idt_set_gate(32, (unsigned long)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned long)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned long)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned long)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned long)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned long)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned long)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned long)irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned long)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned long)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned long)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned long)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned long)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned long)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned long)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned long)irq15, 0x08, 0x8E);

    idt_flush((unsigned long)&idt_ptr);
    asm volatile("sti");
}

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector = sel;
    idt_entries[num].zero = 0;
    idt_entries[num].flags = flags;
}

void register_interrupt_handler(unsigned char n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs) {
    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void irq_handler(registers_t regs) {
    if (regs.int_no >= 40) {
        asm volatile("outb %%al, %%dx" : : "d"(0xA0), "a"(0x20));
    }
    asm volatile("outb %%al, %%dx" : : "d"(0x20), "a"(0x20));

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}
