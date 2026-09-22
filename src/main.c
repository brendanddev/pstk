#include <stdio.h>
#include <stdlib.h>
#include "pstk.h"

int main(void) {
    
    SaveFile *save = load_save("saves/test_write.sav");
    if (save == NULL) {
        printf("Failed to load save\n");
        return 1;
    }

    printf("Save valid: %d, current slot: %d\n", save->is_valid, current_slot(save));

    uint8_t *sb2 = find_sector(save, 0);
    if (sb2 != NULL) {
        for (int i = 0; i < 8; i++) sb2[i] = 0xFF;
        // A            S               H                       FF
        sb2[0] = 0xBB; sb2[1] = 0xCD; sb2[2] = 0xC2; sb2[3] = 0xFF;

        if (write_save(save, "saves/test_write.sav")) {
            printf("Wrote saves/test_write.sav\n");
        } else {
            printf("Write failed\n");
        }
    }

    free(save);
    save = load_save("saves/test_write.sav");
    if (save != NULL) {
        printf("Reload valid: %d\n", save->is_valid);
        char name[9];
        if (get_player_name(save, name)) {
            printf("Reloaded trainer name: %s\n", name);
        }
    }
    
    free(save);
    return 0;
}
