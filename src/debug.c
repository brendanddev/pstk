#include <stdio.h>
#include "debug.h"
#include "sector.h"
#include "text.h"
#include "bytes.h"

// Classic hex dump: offset, 16 bytes per row, then a text column.
static void dump_bytes(const uint8_t *data, size_t length) {
    for (size_t off = 0; off < length; off += 16) {
        printf("%04zX: ", off);
        for (size_t i = 0; i < 16; i++) {
            if (off + i < length) printf("%02X ", data[off + i]);
            else                  printf("   ");
            if (i == 7) printf(" ");
        }
        printf(" |");
        for (size_t i = 0; i < 16 && off + i < length; i++) {
            uint8_t b = data[off + i];
            char c = gen3_to_ascii(b);
            printf("%c", (b == 0x00 || c == '?') ? '.' : c);
        }
        printf("|\n");
    }
}

void print_sectors(const SaveFile *save) {
    for (int i = 0; i < SECTOR_COUNT; i++) {
        const uint8_t *sec = sector_at(save, i);
        printf("pos %2d: id=%5u counter=%10u stored=%04X ok=%d\n",
            i,
            (unsigned)rd16(sec + FOOTER_ID_OFFSET),
            (unsigned)rd32(sec + FOOTER_COUNTER_OFFSET),
            (unsigned)rd16(sec + FOOTER_CHECKSUM_OFFSET),
            verify_sector(save, i));
    }
}

void dump_sector(SaveFile *save, uint16_t id, size_t length) {
    size_t max = sector_data_size(id);
    if (max == 0) {
        printf("Invalid sector ID %u\n", (unsigned)id);
        return;
    }

    const uint8_t *sec = find_sector(save, id);
    if (sec == NULL) {
        printf("Sector ID %u not found\n", (unsigned)id);
        return;
    }

    if (length == 0 || length > max) length = max;
    printf("Sector ID %u (first %zu bytes)\n", (unsigned)id, length);
    dump_bytes(sec, length);
}
