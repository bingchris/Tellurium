#include "../Include/framebuffer.h"
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
        framebuffer[y * pitch + x] = color;
    }
}
