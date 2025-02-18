#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "multiboot.h"

void draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void initialize_framebuffer(MultibootInfo* mb_info);

#endif // FRAMEBUFFER_H
