#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "save.h"

typedef struct PlayTime {
    uint16_t hours;
    uint8_t  minutes;
    uint8_t  seconds;
} PlayTime;

// Writes the trainer name (up to 8 chars + NUL) into `out`.
// Returns 1 on success, 0 if the player info sector is missing.
int get_player_name(SaveFile *save, char out[9]);

// Returns 0 (male), 1 (female), or -1 if the player info sector is missing.
int get_player_gender(SaveFile *save);

// Visible trainer ID (shown on the trainer card) and secret ID. Returns 1 on success.
int get_trainer_id(SaveFile *save, uint16_t *tid, uint16_t *sid);

// Returns 1 on success.
int get_play_time(SaveFile *save, PlayTime *out);

#endif
