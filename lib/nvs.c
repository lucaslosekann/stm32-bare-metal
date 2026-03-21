#include "nvs.h"

static uint32_t nvs_write_ptr = NVS_START;

static uint32_t nvs_hash(const char *s) {
    uint32_t h = 5381;
    while (*s)
        h = ((h << 5) + h) + (uint8_t)*s++;
    return h;
}

static uint32_t nvs_crc(const void *data, uint16_t len) {
    const uint8_t *p = data;
    uint32_t crc = 0;
    for (uint16_t i = 0; i < len; i++)
        crc ^= p[i];
    return crc;
}

void nvs_init(void) {
    // Finds the first not used address in the sector
    uint32_t addr = NVS_START;

    while (addr < NVS_END) {
        nvs_entry_t *e = (nvs_entry_t *)addr;

        if (e->crc == 0xffffffff && e->key_hash == 0xffffffff) break;

        addr += sizeof(nvs_entry_t);
    }

    nvs_write_ptr = addr;
}

int nvs_get_address(const char *key, uint32_t *out_addr) {
    uint32_t hash = nvs_hash(key);
    uint32_t addr = NVS_START;

    while (addr < nvs_write_ptr) {
        nvs_entry_t *e = (nvs_entry_t *)addr;

        if (e->valid == 0x0000FFFF && e->key_hash == hash) {
            uint32_t val = e->data;

            if (nvs_crc(&val, 4) == e->crc) {
                *out_addr = addr;
                return CODE_OK;
            }
        }

        addr += sizeof(nvs_entry_t);
    }
    return CODE_NOT_FOUND;
}

int nvs_set(const char *key, uint32_t value) {
    uint32_t hash = nvs_hash(key);
    if (nvs_write_ptr + sizeof(nvs_entry_t) + 1 >= NVS_END) {
        return CODE_NVS_FULL;
    }
    nvs_entry_t entry = {
        .key_hash = hash,
        .valid = 0xFFFFFFFF,
        .crc = nvs_crc(&value, 4),
        .data = value,
    };

    flash_unlock();

    // find another with same hash
    uint32_t existing_address;
    int found = nvs_get_address(key, &existing_address) == CODE_OK;

    // make it invalid
    if (found) {
        flash_write32(existing_address + 4, 0x00000000);
    }

    // write entry
    for (unsigned int i = 0; i < sizeof(entry) / 4; i++)
        flash_write32(nvs_write_ptr + i * 4, ((uint32_t *)&entry)[i]);

    // commit (valid = 0)
    flash_write32(nvs_write_ptr + 4, 0x0000FFFF); // offset do campo valid

    flash_lock();

    nvs_write_ptr += sizeof(entry);

    return CODE_OK;
}

int nvs_get(const char *key, uint32_t *out) {
    uint32_t addr;
    int code = nvs_get_address(key, &addr);
    if (code != CODE_OK) {
        return code;
    }

    nvs_entry_t *e = (nvs_entry_t *)addr;
    *out = e->data;
    return CODE_OK;
}

int nvs_erase(void) { return flash_erase_sector(7); }