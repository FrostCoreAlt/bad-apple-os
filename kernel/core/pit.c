#include "core/io.h"
#include <stdint.h>

#define PIT_FREQUENCY 1193182
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43

// Sleep for approx `ms` milliseconds
void pit_sleep_ms(uint32_t ms) {
    if (ms == 0) return;

    // Calculate number of PIT ticks to wait
    uint32_t ticks_to_wait = (PIT_FREQUENCY * ms) / 1000;
    if (ticks_to_wait == 0) ticks_to_wait = 1;
    if (ticks_to_wait > 0xFFFF) ticks_to_wait = 0xFFFF; // Max for 16-bit counter

    // Set PIT to mode 0 (interrupt on terminal count), channel 0, binary counting
    // Command byte: 00 (channel 0) | 11 (lobyte/hibyte) | 000 (mode 0) | 0 (binary)
    outb(PIT_COMMAND, 0x30);

    // Load divisor - low byte then high byte
    outb(PIT_CHANNEL0, ticks_to_wait & 0xFF);        // low byte
    outb(PIT_CHANNEL0, (ticks_to_wait >> 8) & 0xFF); // high byte

    // Wait for the counter to reach 0
    // In mode 0, the output pin goes high when counter reaches 0
    // We can poll the status to check when counting is complete
    while (1) {
        // Send latch command and read back status
        outb(PIT_COMMAND, 0xE2); // Latch status for channel 0
        
        uint8_t status = inb(PIT_CHANNEL0);
        
        // Check if output pin is high (bit 7 of status)
        if (status & 0x80) {
            break;
        }
    }
}
