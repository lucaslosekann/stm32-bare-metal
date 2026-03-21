#ifndef __SYSCLOCK_H
#define __SYSCLOCK_H

#include "f411.h"

#define HSE_SPEED_MHZ 25

enum {
    PLL_M = 25,
    PLL_N = 192,
    PLL_P = 2,
    PLL_Q = 4,
};

#define APB1_DIV APB_DIV2
#define FLASH_LATENCY 3
#define SYS_FREQUENCY ((HSE_SPEED_MHZ * PLL_N / PLL_M / PLL_P) * 1000000)

#define APB_DIV1 0b000
#define APB_DIV2 0b100
#define APB_DIV4 0b101
#define APB_DIV8 0b110
#define APB_DIV16 0b111

#define APB1_SHIFT 10
#define APB2_SHIFT 13

#define PLL_RESERVED 0xF0BC8000
#define PLL_N_SHIFT 6
#define PLL_P_SHIFT 16
#define PLL_Q_SHIFT 24

#define PLL_VAL (PLL_M | (PLL_N << PLL_N_SHIFT) | (((PLL_P >> 1) - 1) << PLL_P_SHIFT) | (4 << PLL_Q_SHIFT))

/* RCC->CFGR */
#define CONF_CLOCK_BITS 0x3
#define CONF_HSI 0x0
#define CONF_HSE 0x1
#define CONF_PLL 0x2

/* RCC->CR */
#define HSEON BIT(16)
#define HSERDY BIT(17)
#define PLLON BIT(24)
#define PLLRDY BIT(25)

/* RCC->PLLCFGR */
#define SRCHSE BIT(22)

void sysclock_init(void);

#endif /* __SYSCLOCK_H */