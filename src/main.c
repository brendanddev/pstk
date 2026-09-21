#include <stdio.h>
#include <stdlib.h>
#include "pkmnemerald.h"

int main(void) {
    SaveFile *save = load_save("saves/test.sav");
    if (save == NULL) {
        printf("Failed to load save\n");
        return 1;
    }

    printf("Save valid: %d, current slot: %d\n", save->is_valid, current_slot(save));

    char name[9];
    PlayTime pt;
    uint16_t tid, sid;
    if (get_player_name(save, name) && get_trainer_id(save, &tid, &sid)
        && get_play_time(save, &pt)) {
        printf("Trainer: %s (%s)\n", name, get_player_gender(save) == 0 ? "boy" : "girl");
        printf("ID: %05u (secret %05u)\n", tid, sid);
        printf("Play time: %u:%02u:%02u\n", pt.hours, pt.minutes, pt.seconds);
    }

    dump_sector(save, 0, 64);

    free(save);
    return 0;
}
