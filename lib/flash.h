#ifndef __FLASH_H
#define __FLASH_H

#include "f411.h"

// Flash peripheral structure
struct flash {
    volatile uint32_t ACR;     // Access control register
    volatile uint32_t KEYR;    // Key register
    volatile uint32_t OPTKEYR; // Option key register
    volatile uint32_t SR;      // Status register
    volatile uint32_t CR;      // Control register
    volatile uint32_t OPTCR;   // Option control register
};

#define FLASH ((struct flash *)0x40023C00) // Flash base address

#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xCDEF89AB

// ACR BITS
#define FL_PRFTEN BIT(10)
#define FL_ICEN BIT(11)
#define FL_DCEN BIT(12)
#define FL_ICRST BIT(13)
#define FL_DCRST BIT(14)

// CR BITS
#define FL_LOCK BIT(31)
#define FL_PG BIT(0)
#define FL_SER BIT(1)
#define FL_STRT BIT(16)

// SR BITS
#define FL_BSY BIT(16)

void set_flash_latency(int latency);
void flash_cache_enable(void);

void flash_unlock(void);
void flash_write32(uint32_t addr, uint32_t data);
void flash_lock(void);
int flash_erase_sector(uint8_t sector);

#endif /* __FLASH_H */