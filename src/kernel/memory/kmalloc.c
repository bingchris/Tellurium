#include "kmalloc.h"
#include <stdint.h>
/*
	Keystone kernel
	bingchris 2025
*/
// block header
typedef struct block_header {
    size_t size;                  
    struct block_header *next;    
} block_header_t;

// heap meta
static uint8_t *heap_base = NULL;
static uint8_t *heap_end = NULL;
static block_header_t *freelist = NULL;

static size_t align_size(size_t size) {
    return (size + 7) & ~7;
}

void kmalloc_init(void *heap_start, size_t heap_size) {
    heap_base = (uint8_t *)heap_start;
    heap_end = heap_base + heap_size;

    freelist = (block_header_t *)heap_base;
    freelist->size = heap_size - sizeof(block_header_t);
    freelist->next = NULL;
}

// alloc
void *kmalloc(size_t size) {
    size = align_size(size);

    block_header_t **current = &freelist;
    while (*current) {
        if ((*current)->size >= size) {
            block_header_t *allocated = *current;

            if (allocated->size > size + sizeof(block_header_t)) {
                block_header_t *new_block = (block_header_t *)((uint8_t *)allocated + sizeof(block_header_t) + size);
                new_block->size = allocated->size - size - sizeof(block_header_t);
                new_block->next = allocated->next;

                allocated->size = size;
                *current = new_block;
            } else {
                *current = allocated->next;
            }

            return (void *)((uint8_t *)allocated + sizeof(block_header_t));
        }

        current = &(*current)->next;
    }

    // oom(f) (sorry)
    return NULL;
}

void kfree(void *ptr) {
    if (!ptr) return;

    block_header_t *block = (block_header_t *)((uint8_t *)ptr - sizeof(block_header_t));

    block->next = freelist;
    freelist = block;
}
