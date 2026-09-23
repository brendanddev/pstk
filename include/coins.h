#ifndef COINS_H
#define COINS_H

#include <stdint.h>
#include "save.h"

#define MAX_COINS 9999

// Reads decrypted coin count. Returns 1 on success, 0 if a required
// sector is missing.
int get_coins(SaveFile *save, uint16_t *out);

// Writes coin count (encrypted with the saves current key). Returns 0
// if a required sector is missing or value exceeds MAX_COINS.
int set_coins(SaveFile *save, uint16_t value);

#endif
