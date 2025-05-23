#include <stdint.h>
#include "keyboard.h"
#include "kernel/io.h"

static uint8_t buffer[KEYBOARD_BUFFER_SIZE];
static uint8_t buffer_head = 0;
static uint8_t buffer_tail = 0;
// turn raw input into ascii using a char{}
static const char keymap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

void keyboard_init() { //honestly i was having no idea on how to fix keyboard so i just copied from my old os project
    /* send ICW1 */
    outb(PIC1, ICW1);
    outb(PIC2, ICW1);
    /* send ICW2 */
    outb(PIC1 + 1, PIC_1);   
    outb(PIC2 + 1, PIC_2);   
    /* send ICW3 */
    outb(PIC1 + 1, 4);   
    outb(PIC2 + 1, 2);
    /* send ICW4 */
    outb(PIC1 + 1, ICW4);
    outb(PIC2 + 1, ICW4);
    /* disable all IRQs */
    outb(PIC1 + 1, 0xFF);
}

void keyboard_read() {
    uint8_t status = inb(KEYBOARD_STATUS_PORT);

    if (status & 0x01) {
        buffer[buffer_head] = inb(KEYBOARD_DATA_PORT);
        buffer_head = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
    }
}
uint8_t keyboard_get_key() {
    if (buffer_head == buffer_tail) {
        return 0; // buff emp
    } else {
        uint8_t key = buffer[buffer_tail];
        buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
        return key;
    }
}
char keyboard_get_ascii() {
    uint8_t key = keyboard_get_key();
    if (key < 128) {
        return keymap[key];
    } else {
        return 0; // not sigma (how the fuck do you have a key outside 128 range)
    }
}
void keyboard_clear_buffer() {
    buffer_head = 0;
    buffer_tail = 0;
}
uint8_t keyboard_buffer_size() {
    return (buffer_head - buffer_tail + KEYBOARD_BUFFER_SIZE) % KEYBOARD_BUFFER_SIZE;
}