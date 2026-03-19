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

// ACR BITS
#define FL_PRFTEN BIT(10)
#define FL_ICEN BIT(11)
#define FL_DCEN BIT(12)
#define FL_ICRST BIT(13)
#define FL_DCRST BIT(14)

void set_flash_latency(int latency);
void flash_cache_enable(void);

#endif /* __FLASH_H */