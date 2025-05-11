#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"
#include "framebuffer/framebuffer.h"
#include "limine_setup.h"
/*
	Keystone kernel
	bingchris 2025
*/

// base revision 3 (latest)
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// framebuffer request i think
__attribute__((used, section(".limine_requests")))



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

    // Initialize Limine framebuffer
    setup_limine();

    // Ensure framebuffer exists
    struct framebuffer *fb = get_framebuffer();
    if (!fb) {
        hcf();  // Halt if framebuffer isn't available
    }

    // Print "Hello, world!"
    kprint("Yep, that's text in keystone.\nyay.", 0x00ff00);

    // Halt cleanly
    for (;;) asm ("hlt");
}