#include "money.h"
#include "bytes.h"

// SaveBlock 1
#define SB1_MONEY_OFFSET          0x490

int get_money(SaveFile *save, uint32_t *out) {
    uint32_t key;
    if (!get_encryption_key(save, &key)) return 0;

    uint8_t *p = sb1_ptr(save, SB1_MONEY_OFFSET);
    if (p == NULL) return 0;

    // Decrypt using XOR to undo encryption
    *out = rd32(p) ^ key;
    return 1;
}

int set_money(SaveFile *save, uint32_t value) {
    if (value > MAX_MONEY) return 0;

    uint32_t key;
    if (!get_encryption_key(save, &key)) return 0;

    uint8_t *p = sb1_ptr(save, SB1_MONEY_OFFSET);
    if (p == NULL) return 0;

    // Encrypt using the same key via XOR
    wr32(p, value ^ key);
    return 1;
}
