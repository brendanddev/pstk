#ifndef FLAGS_H
#define FLAGS_H

#include <stdint.h>
#include "save.h"

// Number of defined flag IDs (id=0 is a null sentinel)
#define FLAGS_COUNT 0x960

// Reads flag `id` (1-based; 0 and id >= FLAGS_COUNT are rejected).
// Returns 1 on success, 0 on invalid id or missing sector.
int get_flag(SaveFile *save, uint16_t id, int *out);

// Sets flag `id` to `value` (nonzero = set). Returns 1 on success, 0 on
// invalid id or missing sector.
int set_flag(SaveFile *save, uint16_t id, int value);

#endif
