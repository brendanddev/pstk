#ifndef MONEY_H
#define MONEY_H

#include <stdint.h>
#include "save.h"

#define MAX_MONEY 999999

// Reads the players decrypted money. Returns 1 on success, 0 if the
// player info or SaveBlock1 sector is missing.
int get_money(SaveFile *save, uint32_t *out);

// Writes money (encrypted with the saves current key). Returns 0 if a
// required sector is missing or value exceeds MAX_MONEY.
int set_money(SaveFile *save, uint32_t value);

#endif
