#include <stdio.h>
#include <stdlib.h>
#include "save.h"
#include "sector.h"
#include "bytes.h"

// Returns the slot's save counter, or -1 if its first sector has no valid signature.
static int64_t slot_counter(const SaveFile *save, int slot) {
    const uint8_t *sec = sector_at(save, slot * SECTORS_PER_SLOT);
    if (rd32(sec + FOOTER_SIGNATURE_OFFSET) != SECTOR_SIGNATURE) return -1;
    return rd32(sec + FOOTER_COUNTER_OFFSET);
}

SaveFile *load_save(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) return NULL;

    SaveFile *save = malloc(sizeof(SaveFile));
    if (save == NULL) {
        fclose(file);
        return NULL;
    }

    // Read at most SAVE_SIZE bytes; extra emulator data is ignored.
    save->size = fread(save->data, 1, SAVE_SIZE, file);

    save->is_valid = (save->size == SAVE_SIZE)
                  && verify_slot(save, current_slot(save));

    fclose(file);
    return save;
}

int verify_slot(const SaveFile *save, int slot) {
    for (int i = 0; i < SECTORS_PER_SLOT; i++) {
        if (!verify_sector(save, slot * SECTORS_PER_SLOT + i)) return 0;
    }
    return 1;
}

// The slot with the higher counter is the most recent save.
int current_slot(const SaveFile *save) {
    return slot_counter(save, 1) > slot_counter(save, 0) ? 1 : 0;
}

uint8_t *find_sector(SaveFile *save, uint16_t id) {
    int base = current_slot(save) * SECTORS_PER_SLOT;
    for (int i = base; i < base + SECTORS_PER_SLOT; i++) {
        const uint8_t *sec = sector_at(save, i);
        if (rd32(sec + FOOTER_SIGNATURE_OFFSET) == SECTOR_SIGNATURE
            && rd16(sec + FOOTER_ID_OFFSET) == id) {
            return save->data + (size_t)i * SECTOR_SIZE;
        }
    }
    return NULL;
}
