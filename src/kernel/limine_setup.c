#include "limine_setup.h"
#include <stddef.h>
#include "framebuffer/framebuffer.h"
// Limine framebuffer request
struct limine_framebuffer_request framebuffer_request __attribute__((used)) = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
static struct framebuffer fb_info;  // Make struct global for persistence

struct framebuffer *get_framebuffer() {
    
    if (!framebuffer_request.response || framebuffer_request.response->framebuffer_count < 1) {
        return NULL;  // No framebuffer available
    }

    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

    // Ensure proper initialization
    fb_info.address = (uint32_t *)fb->address;
    fb_info.width = fb->width;
    fb_info.height = fb->height;
    fb_info.pitch = fb->pitch;

    return &fb_info;  // Return pointer to valid framebuffer struct
}
void setup_limine() {
    struct framebuffer *fb = get_framebuffer();
    if (!fb) return;

    framebuffer_clear(0x000000);  // Black background on boot
}