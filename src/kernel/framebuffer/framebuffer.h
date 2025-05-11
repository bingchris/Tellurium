#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
/*
	Keystone kernel
	bingchris 2025
*/
#include <stdint.h>

void framebuffer_clear(uint32_t color);
void framebuffer_set_pixel(uint16_t x, uint16_t y, uint32_t color);
void scroll_framebuffer();  
void kprint(const char *str, uint32_t color);
uint32_t framebuffer_get_pixel(uint16_t x, uint16_t y); 

#endif
