#include "flash.h"

void set_flash_latency(int latency) {
    FLASH->ACR = (FLASH->ACR & ~(uint32_t)0xf) | (uint32_t)latency; // Clear latency bits and then set them
}

void flash_cache_enable(void) {
    FLASH->ACR |= FL_PRFTEN | FL_ICEN | FL_DCEN; // Enable prefetch, instruction cache and data cache
}
