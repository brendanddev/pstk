#ifndef BAG_H
#define BAG_H

#include <stdint.h>
#include "save.h"

// Storage order in SaveBlock1, not the games internal pocket numbering 
// (which is Items=0, Balls=1, TMHM=2, Berries=3, KeyItems=4). Cannot assume
// this enums values match anything in the decomp, its all internal.
typedef enum {
    POCKET_ITEMS,
    POCKET_KEY_ITEMS,
    POCKET_BALLS,
    POCKET_TMHM,
    POCKET_BERRIES,
    POCKET_COUNT
} Pocket;

// A single bag slot, always in decrypted form.
// Empty slot = id 0, quantity 0 (the raw on-disk quantity for an empty slot is NOT zero).
typedef struct {
    uint16_t id;
    uint16_t quantity;
} ItemSlot;

// Number of slots in a pocket, or 0 if `pocket` is invalid.
int pocket_capacity(Pocket pocket);

// Reads slot `index` (0-based) of `pocket` into *out, decrypted.
// Returns 1 on success, 0 if the pocket/index is invalid or the sector
// is missing.
int bag_get_slot(SaveFile *save, Pocket pocket, int index, ItemSlot *out);

// Writes slot `index` of `pocket`, encrypting the quantity.
// Returns 0 if the pocket/index is invalid, the sector is missing, or
// quantity exceeds the pocket's max.
int bag_set_slot(SaveFile *save, Pocket pocket, int index, ItemSlot slot);

// Returns the index of the first slot in `pocket` holding item `id`,
// or -1 if not found or on error.
int bag_find_item(SaveFile *save, Pocket pocket, uint16_t id);

#endif
