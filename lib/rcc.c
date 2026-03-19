#include "rcc.h"

void rcc_init(void) {
    RCC->AHB1ENR |= GPIOA_ENABLE;
    RCC->AHB1ENR |= GPIOB_ENABLE;
    RCC->AHB1ENR |= GPIOC_ENABLE;

    RCC->APB2ENR |= SPI1_ENABLE;
}