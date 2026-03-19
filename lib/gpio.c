#include "gpio.h"

void gpio_config_pin(uint16_t pin, gpio_cfg_t cfg) {
    struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO bank
    int n = PINNO(pin);                     // Pin number

    gpio->MODER &= ~(3U << (n * 2));    // Clear existing setting
    gpio->MODER |= cfg.mode << (n * 2); // Set new mode

    gpio->OTYPER &= ~(1U << (n));    // Clear existing setting
    gpio->OTYPER |= cfg.type << (n); // Set new type

    gpio->PUPDR &= ~(3U << (n * 2));    // Clear existing setting
    gpio->PUPDR |= cfg.pupd << (n * 2); // Set io config

    gpio->OSPEEDR &= ~(3U << (n * 2));     // Clear existing setting
    gpio->OSPEEDR |= cfg.speed << (n * 2); // Set speed config
}

void gpio_write(uint16_t pin, bool val) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = BIT(PINNO(pin)) << (val ? 0 : 16);
}

bool gpio_read(uint16_t pin) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    return gpio->IDR & BIT(PINNO(pin)) ? true : false;
}