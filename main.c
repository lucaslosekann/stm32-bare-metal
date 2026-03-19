#include "main.h"

void flash_command(spi_dev_t *dev, uint8_t cmd, uint8_t *data, int size) {
    // CS LOW
    gpio_write(dev->cfg.cs_pin, false);

    // Send command
    spi_write(dev, cmd);

    // Read size bytes
    spi_read(dev, data, size);

    // CS HIGH
    gpio_write(dev->cfg.cs_pin, true);
}

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

    // Config SPI1
    spi_cfg_t spi1_cfg = {
        .baud_rate_control = SPI_BDC_2,
        .type = SPI_TYPE_MASTER,
        .miso_pin = PIN('A', 6),
        .mosi_pin = PIN('A', 7),
        .sck_pin = PIN('A', 5),
        .cs_pin = PIN('A', 4),
    };
    spi_dev_t spi1 = {
        .port = SPI1,
        .cfg = spi1_cfg,
    };
    spi_config(&spi1);

    // Read JEDEC ID (0x9F)
    uint8_t id[3] = {0};
    flash_command(&spi1, 0x9F, id, 3);

    volatile uint32_t jedec = (id[0] << 16) | (id[1] << 8) | id[2];
    if (jedec == 0xEF4017) {
        gpio_write(led_pin, false);
    } else {
        while (true)
            ;
    }

    while (1) {
    }
}
