section .multiboot
    dd 0x1BADB002       ; magic number
    dd 0x00             ; flags
    dd - (0x1BADB002 + 0x00) ; checksum

section .bss
    resb 8192 ; 8 KB stack


section .text
    extern kernel_main

    ; Entry point
    global start
start:
    ; Set up stack
    mov esp, stack_top

    ; Call kernel main function
    call kernel_main
    cli
    hlt

section .bss
stack_top:
