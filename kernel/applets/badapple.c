#include "stdint.h"
#include "core/kernel.h"
#include "core/cursor.h"
#include "core/pit.h"
#include "badapple_frm.h"   
#include "core/printf.h"

void draw_frame_textmode(const uint8_t frame[SCREEN_HEIGHT][SCREEN_WIDTH]) {
    for (uint16_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (uint16_t x = 0; x < SCREEN_WIDTH; x++) {
            uint16_t pos = y * SCREEN_WIDTH + x;
            uint8_t color = frame[y][x] ? 0x0F : 0x00;
            VGA_MEM[pos] = (color << 8) | 0xDB; 
        }
    }
}

void badapplet(void) {
    for (int f = 0; f < 6955; f++) {           
        draw_frame_textmode(badapple_frames[f]);
	pit_sleep_ms(33);
    }
    kclear();
    kprintf("Bad Apple on 80x25 Text mode\n");
    kprintf("By Frost-Core\n");
    kprintf("Thanks for watching!\n");
    while (1) {
        __asm__ volatile("hlt");
    }
}
