[bits 32]
global _start    ; Expose this label to the linker
extern main      ; Tell NASM main is defined in kernel.c

_start:          ; This is the entry point
    call main    ; Jump to your C kernel
    jmp $        ; Infinite loop if main ever returns