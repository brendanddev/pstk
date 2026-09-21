#ifndef SAVE_H
#define SAVE_H

#include <stdint.h>
#include <stddef.h>

// Size of a GBA Emerald save: 128 KB of flash.
// Emulators may append extra data, which is ignored.
#define SAVE_SIZE 131072

// In-memory copy of a `.sav` file. Over 128 KB, so always heap allocate it.
typedef struct SaveFile {
    uint8_t data[SAVE_SIZE];   // Raw bytes of the save file
    size_t size;               // Number of bytes read
    int is_valid;              // Full size AND current slot's checksums all verify
} SaveFile;

// Loads a `.sav` file into a newly allocated SaveFile. Returns NULL on failure.
// The caller must free() the result.
SaveFile *load_save(const char *filepath);

// Returns 1 if every sector in the slot (0 or 1) verifies.
int verify_slot(const SaveFile *save, int slot);

// Returns the slot (0 or 1) with the higher save counter.
int current_slot(const SaveFile *save);

// Returns the sector with the given footer ID in the current slot, or NULL.
uint8_t *find_sector(SaveFile *save, uint16_t id);

#endif
