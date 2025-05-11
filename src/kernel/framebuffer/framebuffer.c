#include "font.h"
#include "framebuffer.h"
#include "kernel/limine_setup.h"
static uint16_t cursor_x = 0, cursor_y = 0;

void scroll_framebuffer() {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    for (uint16_t y = font_vga_8x16.height; y < fb->height; y++) {
        for (uint16_t x = 0; x < fb->width; x++) {
            framebuffer_set_pixel(x, y - font_vga_8x16.height, framebuffer_get_pixel(x, y));
        }
    }

    // Clear last row
    for (uint16_t x = 0; x < fb->width; x++) {
        framebuffer_set_pixel(x, fb->height - font_vga_8x16.height, 0x000000);
    }
}

void kprint(const char *str, uint32_t color) {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    while (*str) {
        if (*str == '\n') {
            cursor_x = 0;
            cursor_y += font_vga_8x16.height;
        } else {
            draw_char(cursor_x, cursor_y, *str, color);
            cursor_x += font_vga_8x16.width;
        }

        // Ensure text doesn't exceed screen width
        if (cursor_x + font_vga_8x16.width >= fb->width) {
            cursor_x = 0;
            cursor_y += font_vga_8x16.height;
        }

        // Scroll if text exceeds screen height
        if (cursor_y + font_vga_8x16.height >= fb->height) {
            scroll_framebuffer();
            cursor_y -= font_vga_8x16.height;
        }

        str++;
    }
}




void framebuffer_set_pixel(uint16_t x, uint16_t y, uint32_t color) {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    uint32_t *pixel = (uint32_t *)((uintptr_t)fb->address + y * fb->pitch + x * sizeof(uint32_t));
    *pixel = color;
}

void framebuffer_clear(uint32_t color) {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    for (uint16_t y = 0; y < fb->height; y++) {
        for (uint16_t x = 0; x < fb->width; x++) {
            framebuffer_set_pixel(x, y, color);
        }
    }
}

uint32_t framebuffer_get_pixel(uint16_t x, uint16_t y) {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return 0;  // Return black if framebuffer doesn't exist

    uint32_t *pixel = (uint32_t *)((uintptr_t)fb->address + y * fb->pitch + x * sizeof(uint32_t));
    return *pixel;
}
