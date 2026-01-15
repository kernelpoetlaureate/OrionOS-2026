gdt_start:
    dq 0x0 ; null descriptor
gdt_code: 
    dw 0xffff, 0x0, 0x9a00, 0x00cf ; code segment
gdt_data: 
    dw 0xffff, 0x0, 0x9200, 0x00cf ; data segment
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

switch_to_pm:
    cli                     ; Clear interrupts
    lgdt [gdt_descriptor]   ; Load GDT
    mov eax, cr0
    or eax, 0x1             ; Set bit 0 in CR0
    mov cr0, eax
    jmp 0x08:BEGIN_PM       ; Far jump to flush pipeline