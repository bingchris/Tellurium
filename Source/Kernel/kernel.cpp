#ifndef telluriumkernel
#define telluriumkernel
#include "framebuffer.cpp"
#include "../Include/multiboot.h"
#include <string.h>
int aprintf(unsigned int line, char *message, ...) {
    unsigned int i = line*(80*2);
    char *vidmem = (char *) 0xb8000;
    while (*message != 0) {

            vidmem[i] = *message;
            i++;
            vidmem[i] = 0x07;
            i++;
        
        message++;
    };
    return 1;
}
MultibootInfo* multiboot_info_ptr;

extern "C" void kernel_main(MultibootInfo* mb_info) {
    
    initialize_framebuffer(mb_info);

    // Debug: Print framebuffer details
    aprintf(1,"Framebuffer Address: 0x%X\n", mb_info->framebuffer_addr);
    aprintf(2,"Framebuffer Pitch: %u\n", mb_info->framebuffer_pitch);
    aprintf(3,"Framebuffer Width: %u\n", mb_info->framebuffer_width);
    aprintf(4,"Framebuffer Height: %u\n", mb_info->framebuffer_height);
    aprintf(5,"Framebuffer BPP: %u\n", mb_info->framebuffer_bpp);

    draw_pixel(100, 100, 0x00FF00);

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
#endif