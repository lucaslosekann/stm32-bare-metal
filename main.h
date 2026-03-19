#ifndef __MAIN_H
#define __MAIN_H

#define HSE_SPEED_MHZ 25
#include "f411.h"

enum {
    PLL_M = 25,
    PLL_N = 192,
    PLL_P = 2,
    PLL_Q = 4,
};

#define APB1_DIV APB_DIV2
#define FLASH_LATENCY 3
#define SYS_FREQUENCY ((HSE_SPEED_MHZ * PLL_N / PLL_M / PLL_P) * 1000000)

#endif /* __MAIN_H */