#ifndef LIMINE_SETUP_H
#define LIMINE_SETUP_H

#include <stdint.h>
#include "limine.h"
/*
	Keystone kernel
	bingchris 2025
*/
extern struct limine_framebuffer_request framebuffer_request;


struct framebuffer {
    uint32_t *address;
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
};

struct framebuffer *get_framebuffer();
void setup_limine();
#endif