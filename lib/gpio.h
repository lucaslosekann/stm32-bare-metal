#ifndef __GPIO_H
#define __GPIO_H
#include "f411.h"

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(bank) ((struct gpio *)(0x40020000 + 0x400 * (bank)))

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_TYPE_PUSH_PULL, GPIO_TYPE_OPEN_DRAIN };
enum { GPIO_PUPD_NONE, GPIO_PUPD_PULL_UP, GPIO_PUPD_PULL_DOWN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_FAST, GPIO_SPEED_HIGH };

typedef struct {
    uint8_t mode;  // GPIO_MODE_INPUT / OUTPUT / AF / ANALOG
    uint8_t pupd;  // optional: 0 = none, 1 = pull-up, 2 = pull-down
    uint8_t type;  // optional: push-pull / open-drain
    uint8_t speed; // optional: low, medium, fast, high
} gpio_cfg_t;

void gpio_config_pin(uint16_t pin, gpio_cfg_t cfg);
void gpio_write(uint16_t pin, bool val);
bool gpio_read(uint16_t pin);
void gpio_set_af(uint16_t pin, uint8_t af);
#endif /* __GPIO_H */