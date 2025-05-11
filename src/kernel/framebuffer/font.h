#ifndef FONT_H
#define FONT_H
/*
	Keystone kernel
	bingchris 2025
*/
#include <stdint.h>
typedef struct {
    uint8_t width;
    uint8_t height;
    const uint8_t (*glyphs)[16];  // Pointer to an array of 16-byte glyphs
} font_t;


void draw_char(uint16_t x, uint16_t y, char c, uint32_t color);  // Ensure declaration

extern const font_t font_vga_8x16;

#endif
