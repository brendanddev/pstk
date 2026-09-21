#include "text.h"

char gen3_to_ascii(uint8_t c) {
    if (c >= 0xBB && c <= 0xD4) return 'A' + (c - 0xBB);   // A-Z
    if (c >= 0xD5 && c <= 0xEE) return 'a' + (c - 0xD5);   // a-z
    if (c >= 0xA1 && c <= 0xAA) return '0' + (c - 0xA1);   // 0-9
    if (c == 0x00) return ' ';
    return '?';
}
