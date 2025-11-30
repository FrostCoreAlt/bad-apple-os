#include "core/io.h"

#define VGA_MEM ((volatile uint16_t*)0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

extern uint16_t cursor_pos;  
void update_cursor(void);    
