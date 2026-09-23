#ifndef BADGES_H
#define BADGES_H

#include <stdint.h>
#include "save.h"

#define NUM_BADGES 8
// Badges are FLAG_BADGE01_GET .. +7 (0x867-0x86E)
#define FLAG_BADGE01_GET 0x867

// n is 1-based (1..8), matching the games BADGE01..BADGE08 naming.
// Returns 1 on success, 0 on invalid n or missing sector.
int get_badge(SaveFile *save, int n, int *out);

// Grants or clears badge n. Only sets FLAG_BADGEnn_GET, does NOT mark the
// gyms trainer as defeated or set any other gym-progress flags, so the gym
// itself will still act as if it hasnt been beaten. This mirrors what the
// badge flag alone controls ingame (HM field-move use, stat boosts,
// obedience, trainer card badge count), nothing more.
int set_badge(SaveFile *save, int n, int value);

// Packs all 8 badges into one byte: bit (n-1) = badge n. This is a
// normalized form for convenience, not the on disk layout, the real
// flags straddle two bytes (badge 1 is bit 7 of one byte, badges 2-8 are
// bits 0-6 of the next).
int get_badges(SaveFile *save, uint8_t *mask);
int set_badges(SaveFile *save, uint8_t mask);

#endif
