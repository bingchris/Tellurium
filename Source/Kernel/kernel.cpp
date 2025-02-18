#include "../Include/framebuffer.h"
#include "../Include/multiboot.h"

extern "C" MultibootInfo* multiboot_info_ptr;

extern "C" void kernel_main(MultibootInfo* mb_info) {
    multiboot_info_ptr = mb_info;
    initialize_framebuffer(mb_info);

    for (uint32_t y = 0; y < 200; ++y) {
        for (uint32_t x = 0; x < 200; ++x) {
            draw_pixel(x, y, 0x00FF00); // Green pixel
        }
    }

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
