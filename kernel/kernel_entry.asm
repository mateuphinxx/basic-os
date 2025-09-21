[BITS 32]

[EXTERN main]
[GLOBAL kernel_entry]

kernel_entry:
    mov eax, 0x10
    mov ds, eax
    mov ss, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    
    mov esp, 0x90000
    mov ebp, esp
    
    mov dword [0xB8008], 0x2F4B2F45
    mov dword [0xB800C], 0x2F4E2F52
    
    call main
    
    mov dword [0xB8010], 0x4F4E4F45
    mov dword [0xB8014], 0x4F204F44
    
    cli
    hlt

%include "kernel/gdt.asm"
