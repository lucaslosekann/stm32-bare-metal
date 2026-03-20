#include "spi.h"
#include "gpio.h"

void spi_config(spi_dev_t *dev) {
    // PINS Config
    gpio_cfg_t sck_pin_cfg = {
        .mode = GPIO_MODE_AF,
        .pupd = GPIO_PUPD_NONE,
        .type = GPIO_TYPE_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
    };
    gpio_cfg_t mosi_pin_cfg = {
        .mode = GPIO_MODE_AF,
        .pupd = GPIO_PUPD_NONE,
        .type = GPIO_TYPE_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
    };
    gpio_cfg_t miso_pin_cfg = {
        .mode = GPIO_MODE_AF,
        .pupd = GPIO_PUPD_NONE,
        .type = GPIO_TYPE_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
    };
    gpio_cfg_t cs_pin_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pupd = GPIO_PUPD_PULL_UP,
        .type = GPIO_TYPE_PUSH_PULL,
        .speed = GPIO_SPEED_HIGH,
    };

    gpio_config_pin(dev->cfg.sck_pin, sck_pin_cfg);
    gpio_config_pin(dev->cfg.mosi_pin, mosi_pin_cfg);
    gpio_config_pin(dev->cfg.miso_pin, miso_pin_cfg);
    gpio_config_pin(dev->cfg.cs_pin, cs_pin_cfg);
    uint8_t AF = 0;
    if (dev->port == SPI1) {
        AF = 5;
    } else {
        // To be implemented: Other ports
        while (true)
            ;
    }
    gpio_set_af(dev->cfg.sck_pin, AF);
    gpio_set_af(dev->cfg.miso_pin, AF);
    gpio_set_af(dev->cfg.mosi_pin, AF);

    gpio_write(dev->cfg.cs_pin, true);

    dev->port->CR1 &= ~((1U << 0) | (1U << 1)); // Mode 0

    // The first item to configure here is the SPI clock divider (baud rate, BR) in the SPI_CR1 register. This uses the APB frequency (the peripheral bus
    // frequency, or fPCLK) as the input for the SPI clock, which can be set to between fPCLK/2 and fPCLK/256 using three bits of resolution. The divider should
    // be picked to achieve a reasonable clock and thus transfer speed for the application.
    dev->port->CR1 &= ~(3U << 3);                               // Reset BR[2:0]
    dev->port->CR1 |= ((dev->cfg.baud_rate_control & 3U) << 3); // Set BR

    // Unless there are special requirements, the default 8-bit data size, default Mode 0 configuration and default MSB-first setting are good default choices
    // that should work with most SPI devices. This then means that only the clock divisor has to be configured in all cases, after which master mode can be
    // enabled in SPI_CR1 (MSTR). The SS pin can then be enabled and set as output by setting SSOE in SPI_CR2.
    dev->port->CR1 &= ~(1U << 2);           // Reset MSTR
    dev->port->CR1 |= dev->cfg.type << (2); // Set MSTR

    // TEMP

    dev->port->CR1 &= ~(1U << 7); // LSBFIRST = 0, MSB first

    dev->port->CR1 |= (1U << 8) | (1U << 9); // SSM=1, SSi=1 -> Software Slave Management

    dev->port->CR1 &= ~(1U << 10); // RXONLY = 0, full-duplex

    dev->port->CR1 &= ~(1U << 11); // DFF=0, 8 bit data

    // END TEMP

    dev->port->CR2 &= ~(1U << 2); // Reset SSOE
    if (dev->cfg.type == SPI_TYPE_MASTER) {
        dev->port->CR2 |= (0x01 & 1U) << (2); // Set SSOE
    }

    // Finally, the SPI peripheral can be enabled by setting SPE (SPI Peripheral Enable) in SPI_CR1.
    dev->port->CR1 |= BIT(6);
}

void spi_write(spi_dev_t *dev, uint8_t data) {

    while (!(dev->port->SR & SPI_SR_TXE))
        ;

    *((volatile uint8_t *)&(dev->port->DR)) = data;

    while (!(dev->port->SR & SPI_SR_RXNE))
        ; // RXNE

    (void)dev->port->DR; // dummy read

    while (dev->port->SR & SPI_SR_BSY)
        ;
}

void spi_read(spi_dev_t *dev, uint8_t *data, int size) {

    for (int i = 0; i < size; i++) {
        while (dev->port->SR & SPI_SR_BSY)
            ;

        dev->port->DR = 0x00;

        while (!(dev->port->SR & SPI_SR_RXNE))
            ;

        *(data + i) = *((volatile uint8_t *)&(dev->port->DR));

        while (dev->port->SR & SPI_SR_BSY)
            ;
    }
}
