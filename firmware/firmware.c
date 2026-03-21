#include "firmware.h"

void systick_handler(void) { // Weak attribute allows user to override this handler
    ++s_ticks;
}

#define BOOTLOADER_SIZE (0x8000U)
int main(void) {
    SCB->VTOR = BOOTLOADER_SIZE; // Relocate vector table address

    // Enable peripherals clock
    rcc_init();

    // // Initialize external clock
    set_flash_latency(3);
    sysclock_init();
    flash_cache_enable();

    // Initialize SysTick
    systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick

    // Config indicator LED
    uint16_t led_pin = PIN('C', 13);
    gpio_cfg_t led_gpio_config = {.mode = GPIO_MODE_OUTPUT, .pupd = 0, .type = 0, .speed = 0};
    gpio_config_pin(led_pin, led_gpio_config);

    bool led_state = false;
    gpio_write(led_pin, led_state);
    uint32_t next_blink = 500;

    while (true) {

        if (s_ticks >= next_blink) {
            next_blink = s_ticks + 500;
            gpio_write(led_pin, led_state); // Toggle LED every ~500ms
            led_state = !led_state;
        }
    }
}
