#include "systick.h"

// Global tick counter (in milliseconds)
volatile uint32_t s_ticks = 0;

// Initialize SysTick timer to generate interrupts every 'ticks' clock cycles
void systick_init(uint32_t ticks) {
    SYSTICK->RVR = ticks - 1;                // Set reload register
    SYSTICK->CVR = 0;                        // Clear current value register
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2); // Enable SysTick, use processor clock
}

// Systick interrupt handler
_weak void systick_handler(void) { // Weak attribute allows user to override this handler
}