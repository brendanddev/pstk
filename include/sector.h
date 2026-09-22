#ifndef SECTOR_H
#define SECTOR_H

#include <stddef.h>
#include <stdint.h>
#include "save.h"

#define SECTOR_SIZE         4096
#define SECTORS_PER_SLOT    14
#define SECTOR_COUNT        32
#define SECTOR_SIGNATURE    0x08012025u

// Footer field offsets within a sector
#define FOOTER_ID_OFFSET         0xFF4
#define FOOTER_CHECKSUM_OFFSET   0xFF6
#define FOOTER_SIGNATURE_OFFSET  0xFF8
#define FOOTER_COUNTER_OFFSET    0xFFC

// Returns the sector at a file position (0-31). Caller ensures its in range.
const uint8_t *sector_at(const SaveFile *save, int position);

// Number of data bytes covered by the checksum for a sector ID, or 0 if invalid.
size_t sector_data_size(uint16_t id);

// The game's sector checksum over `size` bytes.
uint16_t sector_checksum(const uint8_t *data, size_t size);

// Checks one sectors stored checksum against a freshly computed one.
// `position` is the sector's index in the file (0-31), not its footer ID.
int verify_sector(const SaveFile *save, int position);

// Mutable version of sector_at, for writing.
uint8_t *sector_at_mut(SaveFile *save, int position);

// Recomputes and rewrites the checksum in a sector's footer, based on its
// current data. No-op if the sector's ID is invalid.
void update_sector_checksum(SaveFile *save, int position);

#endif
