#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

#define KEYBOARD_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_BUFFER_SIZE 128
#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01
#define PIC_1 0x20
#define PIC_2 0x28

void keyboard_init();
void keyboard_read();
uint8_t keyboard_get_key();
void keyboard_clear_buffer();
uint8_t keyboard_buffer_size();
char keyboard_get_ascii();
#endif