

/**
 * save.c
 * Author: Brendan Dileo
 */

#include <stdio.h>
#include "../include/pkmnemerald.h"


SaveFile *load_save(const char *filepath) {

    // Get pointer to the .sav file
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        return NULL;
    }

    // Allocate memory for the save file
    SaveFile *save_file = malloc(sizeof(SaveFile));
    if (save_file == NULL) {
        fclose(file);
        return NULL;
    }

    // Read the raw bytes into the data and store size in size
    save_file->size = fread(save_file->data, 1, SAVE_SIZE, file);
    save_file->is_valid = (save_file->size == SAVE_SIZE);

    fclose(file);
    file = NULL;
    return save_file;
}

