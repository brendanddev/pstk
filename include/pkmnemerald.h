
/**
 * pkmnemerald.h
 * Author: Brendan Dileo
 */

#ifndef PKMNEMERALD_H
#define PKMNEMERALD_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define SAVE_SIZE 131072            // 128kb - GBA save size

typedef struct {
    uint8_t data[SAVE_SIZE];        // Raw bytes of the save file
    size_t size;                    // Number of bytes read
    int is_valid;                   // Whether the save file loaded correctly
} SaveFile;


SaveFile* load_save(const char *filepath);


#endif 