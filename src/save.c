#include <stdio.h>
#include <stdlib.h>
#include "save.h"
#include "sector.h"
#include "bytes.h"

#define SAVEBLOCK1_SIZE 15752

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
    save->size = fread(save->data, 1, SAVE_BUFFER_SIZE, file);
    save->is_valid = (save->size >= SAVE_SIZE)
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

int write_save(SaveFile *save, const char *filepath) {
    int slot = current_slot(save);

    // Recompute every sectors checksum in the current slot before writing.
    // Sectors with no valid data are skipped.
    for (int i = 0; i < SECTORS_PER_SLOT; i++) {
        update_sector_checksum(save, slot * SECTORS_PER_SLOT + i);
    }

    FILE *file = fopen(filepath, "wb");
    if (file == NULL) return 0;

    size_t written = fwrite(save->data, 1, save->size, file);
    fclose(file);

    return written == save->size;
}

uint8_t *sb1_ptr(SaveFile *save, size_t offset) {
    if (offset >= SAVEBLOCK1_SIZE) return NULL;

    uint16_t sector_id = (uint16_t)(1 + offset / SAVEBLOCK1_CHUNK_SIZE);
    size_t byte_in_sector = offset % SAVEBLOCK1_CHUNK_SIZE;

    uint8_t *sec = find_sector(save, sector_id);
    if (sec == NULL) return NULL;

    return sec + byte_in_sector;
}
