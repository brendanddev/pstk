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

    if (set_money(save, 123456)) {
        if (write_save(save, "saves/test_write.sav")) {
            printf("Wrote money change to saves/test_write.sav\n");
        }
    }

    free(save);
    save = load_save("saves/test_write.sav");
    if (save != NULL) {
        printf("Reload valid: %d\n", save->is_valid);
        uint32_t money2;
        if (get_money(save, &money2)) {
            printf("Reloaded money: %u\n", money2);
        }
    }

    free(save);
    return 0;
}
