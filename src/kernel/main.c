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

// halt
static void halt(void) {
	for (;;) {
		asm ("hlt");
	}
}

void kernel_main(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        halt();
    }

    setup_limine();

    struct framebuffer *fb = get_framebuffer();
    if (!fb) {
        halt(); 
    }

    // print boot messager
    kprint("Keystone 0.1", 0x00ff00);

    // halt
    halt();
}