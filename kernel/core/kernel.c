#include "stdint.h"
#include "io.h"
#include "printf.h"
#include "core/cursor.h"
#include "applets/badapple.h"

__attribute__((section(".multiboot"), aligned(4)))
const uint32_t multiboot_header[] = {
    0x1BADB002, 0x00000003, -(0x1BADB002 + 0x00000003)
};

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25


void scroll_screen(void) {
    if (cursor_pos >= SCREEN_WIDTH * SCREEN_HEIGHT) {
        for (int i = 0; i < SCREEN_WIDTH * (SCREEN_HEIGHT - 1); i++) {
            VGA_MEM[i] = VGA_MEM[i + SCREEN_WIDTH];
        }
        
        int scrolled_line = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH;
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            VGA_MEM[scrolled_line + i] = (0x07 << 8) | ' ';
        }
        
        cursor_pos = scrolled_line;
    }
}

void kputc(char c) {
    if (c == '\n') {
        cursor_pos += SCREEN_WIDTH - (cursor_pos % SCREEN_WIDTH);
    } else if (c == '\b') {
        if (cursor_pos > 0) {
            cursor_pos--;
            VGA_MEM[cursor_pos] = (0x07 << 8) | ' ';
        }
    } else {
        VGA_MEM[cursor_pos] = (0x07 << 8) | c; 
        cursor_pos++; 
    }
    scroll_screen();
    update_cursor();
}

void kclear() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        VGA_MEM[i] = (0x07 << 8) | ' ';
    }
    cursor_pos = 0;
}

void kernel_main() {
    badapplet();
}

void _start() {
    kernel_main();
    while(1) __asm__ volatile("hlt");
}
