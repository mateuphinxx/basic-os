[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    mov [boot_drive], dl
    
    call enable_a20
    sti

    mov si, loading_msg
    call print_string

    mov ah, 0x02
    mov al, 20
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    mov bx, 0x8000
    int 0x13
    jc disk_error

    mov si, kernel_msg
    call print_string

    jmp 0x8000

print_string:
    lodsb
    or al, al
    jz done
    mov ah, 0x0E
    int 0x10
    jmp print_string
done:
    ret

enable_a20:
    call check_a20
    cmp ax, 1
    je a20_done
    
    mov ax, 0x2401
    int 0x15
    
    call check_a20
    cmp ax, 1
    je a20_done
    
    cli
    call wait_8042
    mov al, 0xAD
    out 0x64, al
    
    call wait_8042
    mov al, 0xD0
    out 0x64, al
    
    call wait_8042_data
    in al, 0x60
    push ax
    
    call wait_8042
    mov al, 0xD1
    out 0x64, al
    
    call wait_8042
    pop ax
    or al, 2
    out 0x60, al
    
    call wait_8042
    mov al, 0xAE
    out 0x64, al
    
    call wait_8042
    sti
    
a20_done:
    ret

check_a20:
    pushf
    push ds
    push es
    push di
    push si
    
    cli
    
    xor ax, ax
    mov es, ax
    mov di, 0x0500
    
    mov ax, 0xFFFF
    mov ds, ax
    mov si, 0x0510
    
    mov al, byte [es:di]
    push ax
    
    mov al, byte [ds:si]
    push ax
    
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF
    
    cmp byte [es:di], 0xFF
    
    pop ax
    mov byte [ds:si], al
    
    pop ax
    mov byte [es:di], al
    
    mov ax, 0
    je check_a20_exit
    
    mov ax, 1
    
check_a20_exit:
    pop si
    pop di
    pop es
    pop ds
    popf
    
    ret

wait_8042:
    in al, 0x64
    test al, 2
    jnz wait_8042
    ret

wait_8042_data:
    in al, 0x64
    test al, 1
    jz wait_8042_data
    ret

disk_error:
    mov si, error_msg
    call print_string
    hlt

loading_msg db 'Loading BasicOS...', 13, 10, 0
kernel_msg db 'Kernel loaded, starting...', 13, 10, 0
error_msg db 'Disk Error!', 13, 10, 0
boot_drive db 0

times 510-($-$$) db 0
dw 0xAA55
