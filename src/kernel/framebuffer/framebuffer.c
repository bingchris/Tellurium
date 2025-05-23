#include "font.h"
#include "framebuffer.h"
#include "kernel/limine_setup.h"
#include "kernel/io.h"

static uint16_t cursor_x = 0, cursor_y = 0;

/*
    Keystone kernel
    bingchris 2025
*/

// scrolls framebuffer up when hit bottom
void scroll_framebuffer() {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    for (uint16_t y = font_vga_8x16.height; y < fb->height; y++) {
        for (uint16_t x = 0; x < fb->width; x++) {
            framebuffer_set_pixel(x, y - font_vga_8x16.height, framebuffer_get_pixel(x, y));
        }
    }

    for (uint16_t x = 0; x < fb->width; x++) {
        framebuffer_set_pixel(x, fb->height - font_vga_8x16.height, 0x000000);
    }
}

void update_cursor() {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    for (uint16_t row = 0; row < font_vga_8x16.height; row++) {
        for (uint16_t col = 0; col < font_vga_8x16.width; col++) {
            framebuffer_set_pixel(cursor_x + col, cursor_y + row, 0x000000); // just black bg
        }
    }

    // draw new cursor (just an underscore lol, update this if you want to)
    draw_char(cursor_x, cursor_y, '_', 0xFFFFFF);
}

void handle_backspace() {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    for (uint16_t row = 0; row < font_vga_8x16.height; row++) {
        for (uint16_t col = 0; col < font_vga_8x16.width; col++) {
            framebuffer_set_pixel(cursor_x + col, cursor_y + row, 0x000000);
        }
    }

    if (cursor_x > 0) {
        cursor_x -= font_vga_8x16.width;
    } else if (cursor_y > 0) {
        cursor_y -= font_vga_8x16.height;
        cursor_x = fb->width - font_vga_8x16.width;
    }

    for (uint16_t row = 0; row < font_vga_8x16.height; row++) {
        for (uint16_t col = 0; col < font_vga_8x16.width; col++) {
            framebuffer_set_pixel(cursor_x + col, cursor_y + row, 0x000000);
        }
    }

    update_cursor(); 
}

// prints a string to the framebuffer (honest reaction: holeh moleh)
void kprint(const char *str, uint32_t color) {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    while (*str) {
        for (uint16_t row = 0; row < font_vga_8x16.height; row++) {
            for (uint16_t col = 0; col < font_vga_8x16.width; col++) {
                framebuffer_set_pixel(cursor_x + col, cursor_y + row, 0x000000);
            }
        }

        if (*str == '\b') {
            handle_backspace();
        } else if (*str == '\n') {
            for (uint16_t row = 0; row < font_vga_8x16.height; row++) {
                for (uint16_t col = 0; col < font_vga_8x16.width; col++) {
                    framebuffer_set_pixel(cursor_x + col, cursor_y + row, 0x000000);
                }
            }
            cursor_x = 0;
            cursor_y += font_vga_8x16.height;
        } else {
            draw_char(cursor_x, cursor_y, *str, color);
            cursor_x += font_vga_8x16.width;
        }

        update_cursor();

        if (cursor_x + font_vga_8x16.width >= fb->width) {
            cursor_x = 0;
            cursor_y += font_vga_8x16.height;
        }

        if (cursor_y + font_vga_8x16.height >= fb->height) {
            scroll_framebuffer();
            cursor_y -= font_vga_8x16.height;
        }

        str++;
    }
}

// framebuffer utils
void framebuffer_set_pixel(uint16_t x, uint16_t y, uint32_t color) {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    uint32_t *pixel = (uint32_t *)((uintptr_t)fb->address + y * fb->pitch + x * sizeof(uint32_t));
    *pixel = color;
}

// fills framebuffer with color
void framebuffer_clear(uint32_t color) {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    for (uint16_t y = 0; y < fb->height; y++) {
        for (uint16_t x = 0; x < fb->width; x++) {
            framebuffer_set_pixel(x, y, color);
        }
    }
}

// gets color of pixel
uint32_t framebuffer_get_pixel(uint16_t x, uint16_t y) {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return 0;

    uint32_t *pixel = (uint32_t *)((uintptr_t)fb->address + y * fb->pitch + x * sizeof(uint32_t));
    return *pixel;
}