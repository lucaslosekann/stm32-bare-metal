#include "sysclock.h"

void sysclock_init() {
    /* Turn on HSE oscillator */
    RCC->CR |= HSEON; // HSE ON
    while (!(RCC->CR & HSERDY))
        ;

    /* Configure PLL
        Clean Reserved; Set values; Set source to hse
    */
    RCC->PLLCFGR = (RCC->PLLCFGR & PLL_RESERVED) | PLL_VAL | SRCHSE;

    /* Turn on PLL */
    RCC->CR |= PLLON;           // PLLON
    while (!(RCC->CR & PLLRDY)) // PLLRDY
        ;

    // blinker ();
    /* switch from HSI to PLL */
    RCC->CFGR =
        (RCC->CFGR & ~(uint32_t)CONF_CLOCK_BITS) | CONF_PLL | (APB1_DIV << APB1_SHIFT); // Clear, set pll as src, set divide by 4 on APB Low speed prescaler
}