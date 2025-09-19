[BITS 16]

[EXTERN main]
[GLOBAL kernel_entry]

kernel_entry:
    mov si, debug_msg
    call print_string
    cli
    
    lgdt [gdt_descriptor]
    
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    jmp CODE_SEG:init_pm

[BITS 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov ebp, 0x90000
    mov esp, ebp
    
    call main
    
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

%include "kernel/gdt.asm"
