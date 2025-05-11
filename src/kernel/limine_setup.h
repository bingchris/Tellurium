#ifndef LIMINE_SETUP_H
#define LIMINE_SETUP_H

#include <stdint.h>
#include "limine.h"

extern struct limine_framebuffer_request framebuffer_request;


// Framebuffer metadata
struct framebuffer {
    uint32_t *address;
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
};

struct framebuffer *get_framebuffer();
void setup_limine();
#endif