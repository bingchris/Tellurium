section .multiboot
    dd 0x1BADB002       ; magic number
    dd 0x00             ; flags
    dd - (0x1BADB002 + 0x00) ; checksum

section .bss
    resb 8192 ; 8 KB stack

section .data
    global multiboot_info_ptr
multiboot_info_ptr:
    dd 0 ; Initialize to zero

section .text
    extern kernel_main

    ; Entry point
    global start
start:
    ; Set up stack
    mov esp, stack_top

    ; Get the address of multiboot info from the stack (passed by bootloader)
    pop eax
    mov [multiboot_info_ptr], eax

    ; Call kernel main function
    call kernel_main
    cli
    hlt

section .bss
stack_top:
