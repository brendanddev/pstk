#include "sector.h"
#include "bytes.h"

// Number of data bytes covered by the checksum, indexed by sector ID
static const size_t SECTOR_DATA_SIZE[SECTORS_PER_SLOT] = {
    3884,                                           // ID 0: SaveBlock2 (player info)
    3968, 3968, 3968, 3848,                         // IDs 1-4: SaveBlock1
    3968, 3968, 3968, 3968, 3968, 3968, 3968, 3968, // IDs 5-12: PC storage
    2000                                            // ID 13: PC storage (end)
};

size_t sector_data_size(uint16_t id) {
    return id < SECTORS_PER_SLOT ? SECTOR_DATA_SIZE[id] : 0;
}

const uint8_t *sector_at(const SaveFile *save, int position) {
    return save->data + (size_t)position * SECTOR_SIZE;
}

// Sum the data as little-endian 32-bit words, then fold the two 16-bit halves.
uint16_t sector_checksum(const uint8_t *data, size_t size) {
    uint32_t sum = 0;
    for (size_t i = 0; i + 4 <= size; i += 4) {
        sum += rd32(data + i);   // overflow wraps mod 2^32 by design
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
