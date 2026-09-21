#ifndef DEBUG_H
#define DEBUG_H

#include <stddef.h>
#include <stdint.h>
#include "save.h"

// Prints every sector's footer and whether its checksum verifies.
void print_sectors(const SaveFile *save);

// Hex-dumps the sector with the given ID (current slot). `length` is the number
// of bytes to show; 0 means the sectors whole data area.
void dump_sector(SaveFile *save, uint16_t id, size_t length);

#endif
