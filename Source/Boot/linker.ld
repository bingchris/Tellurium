ENTRY(start)

SECTIONS
{
    . = 0x100000;
    .text : {
        *(.multiboot)
        *(.text)
    }

    .rodata : { *(.rodata) }

    .data : { *(.data) }

    .bss : { *(.bss) }

    /DISCARD/ : { *(.eh_frame) *(.eh_frame_hdr) }
}
