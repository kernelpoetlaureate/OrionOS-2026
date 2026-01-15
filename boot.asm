[BITS 16]
[ORG 0x7C00]

KERNEL_OFFSET equ 0x1000

start:
    mov [BOOT_DRIVE], dl

    ; Set up stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    call load_kernel      ; 1. Load C code from disk
    call load_memory_map  ; 2. Get RAM map from BIOS
    call switch_to_pm     ; 3. Jump to 32-bit mode

    jmp $

; --- FUNCTIONS ---

load_memory_map:
    mov di, 0x8004          ; MEMORY_MAP_DATA
    xor ebx, ebx
    xor bp, bp              ; Counter
.loop:
    mov edx, 0x534D4150    ; 'SMAP'
    mov eax, 0xE820
    mov ecx, 24
    int 0x15
    jc .done
    cmp eax, 0x534D4150
    jne .done
    add di, 24
    inc bp
    test ebx, ebx
    jnz .loop
.done:
    mov [0x8000], bp        ; MEMORY_MAP_ENTRIES
    ret

load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 30              ; Load 30 sectors (approx 15KB)
    mov dl, [BOOT_DRIVE]
    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    int 0x13
    ret

%include "gdt.asm"

[BITS 32]
BEGIN_PM:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE db 0
times 510-($-$$) db 0
dw 0xAA55