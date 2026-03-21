#ifndef __F4X_H
#define __F4X_H

#include <stdbool.h>
#include <stdint.h>

#define BIT(x) (1U << (x))                             // 00000001000 = 1 << 3 = 8
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num)) // For example, PIN('C', 13) would be ((2 << 8) | 13) = 0x20D
#define PINNO(pin) (pin & 0xFF)                        // For example, PINNO(0x20D) would be (0x20D & 0xFF) = 13
#define PINBANK(pin) (pin >> 8)                        // For example, PINBANK(0x20D) would be (0x20D >> 8) = 2 (which corresponds to bank 'C')

#define _weak __attribute__((weak))

#include "flash.h"
#include "gpio.h"
#include "nvs.h"
#include "rcc.h"
#include "return_codes.h"
#include "scb.h"
#include "spi.h"
#include "sysclock.h"
#include "systick.h"

#endif /* __F4X_H */