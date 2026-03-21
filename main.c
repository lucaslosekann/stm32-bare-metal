#include "main.h"

int main(void) {
    // Enable peripherals clock
    rcc_init();

    // Initialize external clock
    set_flash_latency(3);
    sysclock_init();
    flash_cache_enable();

    // Initialize SysTick
    systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick

    // Config indicator LED
    uint16_t led_pin = PIN('C', 13);
    gpio_cfg_t led_gpio_config = {.mode = GPIO_MODE_OUTPUT, .pupd = 0, .type = 0, .speed = 0};
    gpio_config_pin(led_pin, led_gpio_config);
    gpio_write(led_pin, true);

    // nvs_erase();
    nvs_init();

    // nvs_set("bar", 40);
    // nvs_set("foo", 5);

    uint32_t foo = 0;
    nvs_get("foo", &foo);

    uint32_t bar = 0;
    nvs_get("bar", &bar);

    if (bar + foo == 45) {
        gpio_write(led_pin, false);
    } else {
        gpio_write(led_pin, true);
    }

    while (1) {
    }
}
