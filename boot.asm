[BITS 16]
[ORG 0x7C00]

KERNEL_OFFSET equ 0x1000 ; This is where we will load our kernel

start:
    mov [BOOT_DRIVE], dl ; BIOS stores boot drive in DL, save it.

    ; Set up stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; 1. Load the kernel from disk
    call load_kernel

    ; 2. Switch to Protected Mode
    call switch_to_pm

    jmp $ ; Should never reach here

%include "gdt.asm" ; You need to define your GDT segments here

[BITS 16]
load_kernel:
    mov bx, KERNEL_OFFSET ; Destination address
    mov dh, 15            ; Number of sectors to read (increase as kernel grows)
    mov dl, [BOOT_DRIVE]
    mov ah, 0x02          ; BIOS read sector function
    mov al, dh
    mov ch, 0x00          ; Cylinder 0
    mov dh, 0x00          ; Head 0
    mov cl, 0x02          ; Sector 2 (Sector 1 is this bootloader)
    int 0x13
    ret

[BITS 32]
BEGIN_PM:
    ; Now in 32-bit mode!
    ; Set up segment registers for data
    mov ax, 0x10 ; 0x10 is the offset of our DATA segment in GDT
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call KERNEL_OFFSET ; JUMP TO YOUR C KERNEL!
    jmp $

BOOT_DRIVE db 0
times 510-($-$$) db 0
dw 0xAA55