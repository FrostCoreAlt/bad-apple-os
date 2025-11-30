#include "cursor.h"

uint16_t cursor_pos = 0;

void update_cursor() {
    uint16_t pos = cursor_pos;
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
}
