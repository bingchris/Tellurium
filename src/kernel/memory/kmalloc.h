#ifndef KMALLOC_H
#define KMALLOC_H
/*
	Keystone kernel
	bingchris 2025
*/
#include <stddef.h>

void kmalloc_init(void *heap_start, size_t heap_size);
void *kmalloc(size_t size);
void kfree(void *ptr);

#endif
