#include "panic.h"
#include "printf.h"

void kernel_panic(const char* message) {
    __asm__ volatile("cli"); // Disable interrupts
    
    kprintf("\n*** KERNEL PANIC ***\n");
    kprintf("Reason: %s\n", message);
    kprintf("System halted.\n");
    kprintf("Don't worry, the power button always works ;)\n");    
    kprintf("  []  \n");
    kprintf(" [  ] \n");
    kprintf(" [  ] \n");
    kprintf("  []  \n");
    // Hang forever
    while(1) {
        __asm__ volatile("hlt");
    }
}
