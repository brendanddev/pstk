#include "bag.h"
#include "bytes.h"

// { SaveBlock1 offset, slot count, max quantity per slot }
typedef struct {
    size_t   offset;
    int      capacity;
    uint16_t max_quantity;
} PocketInfo;

static const PocketInfo POCKETS[POCKET_COUNT] = {
    [POCKET_ITEMS]     = { 0x560, 30, 99  },
    // Max quantity is 99 in the struct, the game only ever stores 1
    [POCKET_KEY_ITEMS] = { 0x5D8, 30, 99  },
    [POCKET_BALLS]     = { 0x650, 16, 99  },
    [POCKET_TMHM]      = { 0x690, 64, 99  },
    [POCKET_BERRIES]   = { 0x790, 46, 999 },
};

#define ITEM_SLOT_SIZE 4

int pocket_capacity(Pocket pocket) {
    if (pocket < 0 || pocket >= POCKET_COUNT) return 0;
    return POCKETS[pocket].capacity;
}

// Returns a pointer to slot `index` of `pocket` in SaveBlock1, or NULL.
static uint8_t *bag_slot_ptr(SaveFile *save, Pocket pocket, int index) {
    if (pocket < 0 || pocket >= POCKET_COUNT) return NULL;
    if (index < 0 || index >= POCKETS[pocket].capacity) return NULL;

    size_t offset = POCKETS[pocket].offset + (size_t)index * ITEM_SLOT_SIZE;
    return sb1_ptr(save, offset);
}

int bag_get_slot(SaveFile *save, Pocket pocket, int index, ItemSlot *out) {
    uint8_t *p = bag_slot_ptr(save, pocket, index);
    if (p == NULL) return 0;

    uint32_t key;
    if (!get_encryption_key(save, &key)) return 0;

    // Item IDs are never encrypted, only the quantity is, and only with
    // the low 16 bits of the key (which is same as coins).
    out->id       = rd16(p);
    out->quantity = rd16(p + 2) ^ (uint16_t)key;
    return 1;
}

int bag_set_slot(SaveFile *save, Pocket pocket, int index, ItemSlot slot) {
    if (pocket < 0 || pocket >= POCKET_COUNT) return 0;
    if (slot.quantity > POCKETS[pocket].max_quantity) return 0;

    uint8_t *p = bag_slot_ptr(save, pocket, index);
    if (p == NULL) return 0;

    uint32_t key;
    if (!get_encryption_key(save, &key)) return 0;

    // The game encrypts every slot, including empty ones so the raw quantity of
    // an empty slot is (0 ^ key) = keys low 16 bits.
    wr16(p, slot.id);
    wr16(p + 2, slot.quantity ^ (uint16_t)key);
    return 1;
}

int bag_find_item(SaveFile *save, Pocket pocket, uint16_t id) {
    int capacity = pocket_capacity(pocket);
    for (int i = 0; i < capacity; i++) {
        ItemSlot slot;
        if (!bag_get_slot(save, pocket, i, &slot)) return -1;
        if (slot.id == id) return i;
    }
    return -1;
}
