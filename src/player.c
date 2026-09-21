#include "player.h"
#include "text.h"
#include "bytes.h"

// Offsets within SaveBlock2 (sector ID 0)
#define SB2_GENDER_OFFSET     0x08
#define SB2_TRAINER_ID_OFFSET 0x0A
#define SB2_PLAYTIME_OFFSET   0x0E

int get_player_name(SaveFile *save, char out[9]) {
    const uint8_t *sb2 = find_sector(save, 0);
    if (sb2 == NULL) return 0;

    int n = 0;
    for (int i = 0; i < 8 && sb2[i] != 0xFF; i++) {   // 0xFF terminates the name
        out[n++] = gen3_to_ascii(sb2[i]);
    }
    out[n] = '\0';
    return 1;
}

int get_player_gender(SaveFile *save) {
    const uint8_t *sb2 = find_sector(save, 0);
    if (sb2 == NULL) return -1;
    return sb2[SB2_GENDER_OFFSET];
}

int get_trainer_id(SaveFile *save, uint16_t *tid, uint16_t *sid) {
    const uint8_t *sb2 = find_sector(save, 0);
    if (sb2 == NULL) return 0;
    *tid = rd16(sb2 + SB2_TRAINER_ID_OFFSET);
    *sid = rd16(sb2 + SB2_TRAINER_ID_OFFSET + 2);
    return 1;
}

int get_play_time(SaveFile *save, PlayTime *out) {
    const uint8_t *sb2 = find_sector(save, 0);
    if (sb2 == NULL) return 0;
    out->hours   = rd16(sb2 + SB2_PLAYTIME_OFFSET);
    out->minutes = sb2[SB2_PLAYTIME_OFFSET + 2];
    out->seconds = sb2[SB2_PLAYTIME_OFFSET + 3];
    return 1;
}
