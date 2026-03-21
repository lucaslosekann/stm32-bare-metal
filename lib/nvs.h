#ifndef __NVS_H
#define __NVS_H
#include "f411.h"
#define NVS_START 0x08060000
#define NVS_SIZE (128 * 1024) // setor 0
#define NVS_END (NVS_START + NVS_SIZE)

typedef struct {
    uint32_t key_hash;
    uint32_t valid; // 0xFFFFFFFF = invalid, 0x0000FFFF = valid, 0x00000000 = overwritten
    uint32_t crc;
    uint32_t data;
} nvs_entry_t;

void nvs_init(void);
int nvs_set(const char *key, uint32_t value);
int nvs_get(const char *key, uint32_t *value);
int nvs_erase(void);

#endif /* __NVS_H */