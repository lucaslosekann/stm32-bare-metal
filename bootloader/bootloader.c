#include "bootloader.h"

static void jump_to_main(void) {
    typedef void (*void_fn)(void);

    uint32_t *reset_vector_entry = (uint32_t *)(MAIN_APP_START_ADDRESS + 4U);
    uint32_t *reset_vector = (uint32_t *)(*reset_vector_entry);
    void_fn jump_fn = (void_fn)reset_vector;

    jump_fn();
}

static inline void dsb(void) { __asm volatile("dsb"); }

static inline void isb(void) { __asm volatile("isb"); }

__attribute__((section(".noinit"))) volatile uint32_t boot_flag;

void reset_and_go_to_main() {
    boot_flag = BOOT_APP_MAGIC;

    dsb();                                 // ensure write is finished
    isb();                                 // CPU state is clean before reset
    SCB->AIRCR = (0x5FA << 16) | (1 << 2); // Reset
    while (1) {
        // wait for reset
    }
}

static inline void spin(volatile uint32_t count) {
    while (count--)
        (void)0;
}

int main(void) {
    if (boot_flag == BOOT_APP_MAGIC) {
        boot_flag = 0;
        jump_to_main();
    }

    // Enable peripherals clock
    rcc_init();

    // Initialize external clock
    set_flash_latency(3);
    sysclock_init();
    flash_cache_enable();

    // spin(9999999);
    // check flag in nvs
    // if is not set, go to main
    // if is set, copy binary from external flash into app address

    reset_and_go_to_main();

    // Never return
    return 0;
}