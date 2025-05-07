#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"
/*
	Keystone kernel
	bingchris 2024
*/

// base revision 3 (latest)
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// framebuffer request i think
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

// start and end marker
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

#include "libc/memory.h"

// halt and CATCH FIRE!!!
static void hcf(void) {
	for (;;) {
		asm ("hlt");
	}
}

void kernel_main(void) {
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		hcf();
	}

	// ensure framebuf
	if (framebuffer_request.response == NULL
	|| framebuffer_request.response->framebuffer_count < 1) {
		hcf();
	}
	// fetcg framebuffer first
	struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

	// rgb 32 bit pixels
	for (size_t i=0; i<800; i++) {
		volatile uint32_t *fb_ptr = framebuffer->address;
		fb_ptr[i*(framebuffer->pitch / 4) + i] = 0xff0000;
	}
	hcf();
}