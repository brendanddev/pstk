#include "money.h"
#include "bytes.h"

// SaveBlock2 (sector ID 0)
#define SB2_ENCRYPTION_KEY_OFFSET 0xAC
// SaveBlock 1
#define SB1_MONEY_OFFSET          0x490

// Returns 1 and writes the key on success, 0 if SaveBlock2 is missing.
static int get_encryption_key(SaveFile *save, uint32_t *key) {
    const uint8_t *sb2 = find_sector(save, 0);
    if (sb2 == NULL) return 0;
    *key = rd32(sb2 + SB2_ENCRYPTION_KEY_OFFSET);
    return 1;
}

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
