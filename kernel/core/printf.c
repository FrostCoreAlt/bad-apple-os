#include "printf.h"
#include "stdint.h"
#include <stdarg.h>

extern void kputc(char c);

static void print_string(const char* str) {
    while (*str) {
        kputc(*str++);
    }
}

static void print_hex(uint32_t num) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[9];
    buffer[8] = 0;
    
    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    print_string(buffer);
}

static void print_dec(uint32_t num) {
    char buffer[11];
    char* ptr = buffer + 10;
    *ptr = 0;
    
    if (num == 0) {
        kputc('0');
        return;
    }
    
    while (num > 0) {
        *--ptr = '0' + (num % 10);
        num /= 10;
    }
    print_string(ptr);
}

void kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    char* str = va_arg(args, char*);
                    print_string(str);
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    print_dec(num);
                    break;
                }
                case 'x': {
                    uint32_t num = va_arg(args, uint32_t);
                    print_hex(num);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, int);
                    kputc(c);
                    break;
                }
                case '%': {
                    kputc('%');
                    break;
                }
            }
        } else {
            kputc(*fmt);
        }
        fmt++;
    }
    
    va_end(args);
}
