#include <stdio.h>
#include <stdlib.h>
#include "pstk.h"
#include "bytes.h"

static const char *POCKET_NAMES[POCKET_COUNT] = {
    "Items", "Key Items", "Poke Balls", "TMs/HMs", "Berries"
};

int main(void) {
    SaveFile *save = load_save("saves/test.sav");
    if (save == NULL) {
        printf("Failed to load save\n");
        return 1;
    }

    printf("Save valid: %d, current slot: %d\n", save->is_valid, current_slot(save));

    uint32_t money;
    if (get_money(save, &money)) {
        printf("Money: %u\n", money);
    }

    uint16_t coins;
    if (get_coins(save, &coins)) {
        printf("Coins: %u\n", coins);
    }

    uint8_t badges;
    if (get_badges(save, &badges)) {
        printf("Badges: 0x%02X (", badges);
        for (int n = 1; n <= NUM_BADGES; n++) {
            printf("%d", (badges >> (n - 1)) & 1);
        }
        printf(")\n");
    }

    for (int p = 0; p < POCKET_COUNT; p++) {
        printf("-- %s --\n", POCKET_NAMES[p]);
        int cap = pocket_capacity((Pocket)p);
        for (int i = 0; i < cap; i++) {
            ItemSlot slot;
            if (bag_get_slot(save, (Pocket)p, i, &slot) && slot.id != 0) {
                printf("  slot %2d: id=%3u qty=%u\n", i, slot.id, slot.quantity);
            }
        }
    }

    uint32_t key;
    if (get_encryption_key(save, &key)) {
        for (int i = 0; i < pocket_capacity(POCKET_ITEMS); i++) {
            ItemSlot slot;
            if (!bag_get_slot(save, POCKET_ITEMS, i, &slot) || slot.id != 0) continue;

            uint8_t *raw_slot = sb1_ptr(save, 0x560 + i * 4);
            uint16_t raw_qty = rd16(raw_slot + 2);
            printf("Empty-slot check (slot %d): raw qty = %04X, expected = %04X, match = %d\n",
                i, raw_qty, (uint16_t)key, raw_qty == (uint16_t)key);
            break;
        }
    }

    ItemSlot new_slot = { .id = 13, .quantity = 5 };
    if (bag_set_slot(save, POCKET_ITEMS, 0, new_slot)) {
        printf("Set slot 0 to id=%u qty=%u\n", new_slot.id, new_slot.quantity);
    } else {
        printf("Failed to set slot 0\n");
    }

    ItemSlot empty_slot = { .id = 0, .quantity = 0 };
    if (bag_set_slot(save, POCKET_ITEMS, 1, empty_slot)) {
        printf("Cleared slot 1\n");
    } else {
        printf("Failed to clear slot 1\n");
    }

    if (set_badge(save, 4, 1)) {
        printf("Set badge 4\n");
    }
    if (set_coins(save, 500)) {
        printf("Set coins to 500\n");
    }

    if (write_save(save, "saves/test_write.sav")) {
        printf("Wrote bag changes to saves/test_write.sav\n");
    } else {
        printf("Write failed\n");
    }

    free(save);
    save = load_save("saves/test_write.sav");
    if (save != NULL) {
        printf("Reload valid: %d\n", save->is_valid);

        ItemSlot check0;
        if (bag_get_slot(save, POCKET_ITEMS, 0, &check0)) {
            printf("Reloaded slot 0: id=%u qty=%u\n", check0.id, check0.quantity);
        }

        ItemSlot check1;
        if (bag_get_slot(save, POCKET_ITEMS, 1, &check1)) {
            printf("Reloaded slot 1: id=%u qty=%u (should be id=0 qty=0)\n",
                check1.id, check1.quantity);
        }

        uint32_t key2;
        if (get_encryption_key(save, &key2)) {
            uint8_t *raw_slot1 = sb1_ptr(save, 0x560 + 1 * 4);
            uint16_t raw_qty1 = rd16(raw_slot1 + 2);
            printf("Slot 1 raw qty = %04X, expected = %04X, match = %d\n",
                raw_qty1, (uint16_t)key2, raw_qty1 == (uint16_t)key2);
        }

        int badge4;
        if (get_badge(save, 4, &badge4)) {
            printf("Reloaded badge 4: %d\n", badge4);
        }

        uint16_t coins2;
        if (get_coins(save, &coins2)) {
            printf("Reloaded coins: %u\n", coins2);
        }
    }

    free(save);
    return 0;
}
