[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov si, loading_msg
    call print_string

    mov ah, 0x02
    mov al, 10
    mov ch, 0
    mov cl, 2
    mov dh, 0
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

disk_error:
    mov si, error_msg
    call print_string
    hlt

loading_msg db 'Loading BasicOS...', 13, 10, 0
kernel_msg db 'Kernel loaded, starting...', 13, 10, 0
error_msg db 'Disk Error!', 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55
