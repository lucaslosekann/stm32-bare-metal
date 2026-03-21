#include "flash.h"

void set_flash_latency(int latency) {
    FLASH->ACR = (FLASH->ACR & ~(uint32_t)0xf) | (uint32_t)latency; // Clear latency bits and then set them
}

void flash_cache_enable(void) {
    FLASH->ACR |= FL_PRFTEN | FL_ICEN | FL_DCEN; // Enable prefetch, instruction cache and data cache
}

static void flash_wait(void) {
    while (FLASH->SR & FL_BSY)
        ;
}

void flash_unlock() {
    // If is locked, unlock
    if (FLASH->CR & FL_LOCK) {
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

void flash_lock() { FLASH->CR |= FL_LOCK; }

void flash_write32(uint32_t addr, uint32_t data) {
    flash_wait();
    // Activate programming
    FLASH->CR |= FL_PG;

    *(volatile uint32_t *)addr = data;

    flash_wait();

    FLASH->CR &= ~FL_PG;
}

int flash_erase_sector(uint8_t sector) {
    if (sector > 7) {
        return CODE_ERROR;
    }

    flash_wait();
    flash_unlock();

    FLASH->CR &= ~(0xFU << 3); // Clear SNB (sector number)
    FLASH->CR |= (sector << 3);
    FLASH->CR |= FL_SER;
    FLASH->CR |= FL_STRT;

    flash_wait();
    flash_lock();
    FLASH->CR &= ~FL_SER;
    return CODE_OK;
}
