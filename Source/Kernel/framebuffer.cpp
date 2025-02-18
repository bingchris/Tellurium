#ifndef FRAMEBUFFERC_H
#define FRAMEBUFFERC_H

#include "../Include/multiboot.h"

static uint32_t* framebuffer = nullptr;
static uint32_t pitch;
static uint32_t width;
static uint32_t height;

void initialize_framebuffer(MultibootInfo* mb_info) {
    framebuffer = (uint32_t*)mb_info->framebuffer_addr;
    pitch = mb_info->framebuffer_pitch / 4; // Assuming 32-bit color depth
    width = mb_info->framebuffer_width;
    height = mb_info->framebuffer_height;
}

void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x < width && y < height) {
        uint32_t* pixel_addr = framebuffer + y * pitch + x;
        __asm__ __volatile__(
            "movl %0, (%1)"
            :
            : "r"(color), "r"(pixel_addr)
            : "memory"
        );
    }
}

#endif // FRAMEBUFFERC_H
