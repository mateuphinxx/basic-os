[BITS 16]

[EXTERN main]
[GLOBAL kernel_entry]

kernel_entry:
    mov si, debug_msg
    call print_string
    cli
    
    mov si, gdt_msg
    call print_string
    
    mov eax, gdt_start
    add eax, 0x8000
    mov [gdt_descriptor + 2], eax
    lgdt [gdt_descriptor]
    
    mov si, pm_msg
    call print_string
    
    cli
    
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    db 0xEA
    dd start_pm + 0x8000
    dw 0x08

[BITS 32]    
start_pm:
    mov eax, 0x10
    mov ds, eax
    mov ss, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    
    mov ebp, 0x90000
    mov esp, ebp
    
    mov dword [0xB8000], 0x07410741
    mov dword [0xB8004], 0x07410741
    
    mov dword [0xB8008], 0x07420742
    mov dword [0xB800C], 0x07420742
    
    call main
    
    mov dword [0xB8010], 0x07430743
    mov dword [0xB8014], 0x07430743
    
    jmp $

print_string:
    lodsb
    or al, al
    jz done
    mov ah, 0x0E
    int 0x10
    jmp print_string
done:
    ret

debug_msg db 'Kernel entry reached!', 13, 10, 0
gdt_msg db 'Loading GDT...', 13, 10, 0
pm_msg db 'Switching to protected mode...', 13, 10, 0

%include "kernel/gdt.asm"
