
/**
 * main.c
 * Author: Brendan Dileo
 */

#include <stdlib.h>
#include <stdio.h>
#include "../include/pkmnemerald.h"

int main(void) {

    const char *filepath = "saves/test.sav";
    SaveFile *save_file = load_save(filepath);

    if (save_file == NULL) {
        printf("Failed to load save\n");
        return -1;
    }

    printf("Save File is valid: %d\n", save_file->is_valid);

    free(save_file);
    return 0;
}