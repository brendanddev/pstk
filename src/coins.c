#include "coins.h"
#include "bytes.h"

// SaveBlock1
#define SB1_COINS_OFFSET 0x494

int get_coins(SaveFile *save, uint16_t *out) {
    uint32_t key;
    if (!get_encryption_key(save, &key)) return 0;

    uint8_t *p = sb1_ptr(save, SB1_COINS_OFFSET);
    if (p == NULL) return 0;

    *out = rd16(p) ^ (uint16_t)key;
    return 1;
}

int set_coins(SaveFile *save, uint16_t value) {
    if (value > MAX_COINS) return 0;

    uint32_t key;
    if (!get_encryption_key(save, &key)) return 0;

    uint8_t *p = sb1_ptr(save, SB1_COINS_OFFSET);
    if (p == NULL) return 0;

    wr16(p, value ^ (uint16_t)key);
    return 1;
}
