#include "sector.h"
#include "bytes.h"

// Number of data bytes covered by the checksum, indexed by sector ID
static const size_t SECTOR_DATA_SIZE[SECTORS_PER_SLOT] = {
    // ID 0: SaveBlock2 (player info)
    3884,
    // IDs 1-4: SaveBlock1
    3968, 3968, 3968, 3848,
    // IDs 5-12: PC storage
    3968, 3968, 3968, 3968, 3968, 3968, 3968, 3968,
    // ID 13: PC storage (end)
    2000
};

size_t sector_data_size(uint16_t id) {
    return id < SECTORS_PER_SLOT ? SECTOR_DATA_SIZE[id] : 0;
}

const uint8_t *sector_at(const SaveFile *save, int position) {
    return save->data + (size_t)position * SECTOR_SIZE;
}

// Sum the data as little endian 32-bit words, then fold the two 16-bit halves.
uint16_t sector_checksum(const uint8_t *data, size_t size) {
    uint32_t sum = 0;
    for (size_t i = 0; i + 4 <= size; i += 4) {
        // Overflow wraps mod 2^32 by design
        sum += rd32(data + i);
    }
    return (uint16_t)((sum >> 16) + (sum & 0xFFFF));
}

int verify_sector(const SaveFile *save, int position) {
    if (position < 0 || position >= SECTOR_COUNT) return 0;

    const uint8_t *sec = sector_at(save, position);
    if (rd32(sec + FOOTER_SIGNATURE_OFFSET) != SECTOR_SIGNATURE) return 0;

    size_t size = sector_data_size(rd16(sec + FOOTER_ID_OFFSET));
    if (size == 0) return 0;

    return sector_checksum(sec, size) == rd16(sec + FOOTER_CHECKSUM_OFFSET);
}

uint8_t *sector_at_mut(SaveFile *save, int position) {
    return save->data + (size_t)position * SECTOR_SIZE;
}

void update_sector_checksum(SaveFile *save, int position) {
    uint8_t *sec = sector_at_mut(save, position);
    uint16_t id = rd16(sec + FOOTER_ID_OFFSET);

    size_t size = sector_data_size(id);
    // Invalid/unused sector, nothing to checksum
    if (size == 0) return;

    uint16_t sum = sector_checksum(sec, size);
    sec[FOOTER_CHECKSUM_OFFSET]     = (uint8_t)(sum & 0xFF);
    sec[FOOTER_CHECKSUM_OFFSET + 1] = (uint8_t)(sum >> 8);
}
