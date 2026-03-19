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

    uint16_t led = PIN('C', 13); // Blue LED
    gpio_cfg_t led_gpio_config = {.mode = GPIO_MODE_OUTPUT, .pupd = 0, .type = 0, .speed = 0};
    gpio_config_pin(led, led_gpio_config); // Set blue LED to output mode
    gpio_write(led, true);

    uint16_t sck_pin = PIN('A', 5);
    uint16_t mosi_pin = PIN('A', 7);
    uint16_t miso_pin = PIN('A', 6);
    uint16_t cs_pin = PIN('A', 4);

    spi_cfg_t spi1_cfg = {
        .baud_rate_control = SPI_BDC_64,
        .type = SPI_TYPE_MASTER,
        .miso_pin = miso_pin,
        .mosi_pin = mosi_pin,
        .sck_pin = sck_pin,
        .cs_pin = cs_pin,
    };

    spi_dev_t spi1 = {
        .port = SPI1,
        .cfg = spi1_cfg,
    };

    spi_config(&spi1);

    // 4. Read JEDEC ID (0x9F)
    uint8_t id[3] = {0};
    flash_command(&spi1, 0x9F, id, 3);

    volatile uint32_t jedec = (id[0] << 16) | (id[1] << 8) | id[2];
    if (jedec == 0xEF4017) {
        gpio_write(led, false);
    } else {
        while (true)
            ;
    }

    while (1) {
    }
}
