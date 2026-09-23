#include "flags.h"
#include "sector.h"

// SaveBlock1: u8 flags[NUM_FLAG_BYTES]
#define FLAGS_OFFSET 0x1270

int get_flag(SaveFile *save, uint16_t id, int *out) {
    if (id == 0 || id >= FLAGS_COUNT) return 0;

    uint8_t *p = sb1_ptr(save, FLAGS_OFFSET + id / 8);
    if (p == NULL) return 0;

    // LSB first, as per mentioned in the decomps FlagGet
    *out = (*p >> (id & 7)) & 1;
    return 1;
}

int set_flag(SaveFile *save, uint16_t id, int value) {
    if (id == 0 || id >= FLAGS_COUNT) return 0;

    uint8_t *p = sb1_ptr(save, FLAGS_OFFSET + id / 8);
    if (p == NULL) return 0;

    uint8_t bit = 1 << (id & 7);
    if (value) *p |= bit;
    else       *p &= (uint8_t)~bit;
    return 1;
}
