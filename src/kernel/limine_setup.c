#include "limine_setup.h"
#include <stddef.h>
#include "framebuffer/framebuffer.h"
/*
	Keystone kernel
	bingchris 2025
*/
// request
struct limine_framebuffer_request framebuffer_request __attribute__((used)) = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
static struct framebuffer fb_info;

struct framebuffer *get_framebuffer() {
    
    if (!framebuffer_request.response || framebuffer_request.response->framebuffer_count < 1) {
        return NULL; 
    }

    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

    fb_info.address = (uint32_t *)fb->address;
    fb_info.width = fb->width;
    fb_info.height = fb->height;
    fb_info.pitch = fb->pitch;

    return &fb_info;  
}
void setup_limine() {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    framebuffer_clear(0x000000); 
}