#include "badges.h"
#include "flags.h"

// Returns 0 if n is out of range, otherwise the flag ID for badge n.
static uint16_t badge_flag_id(int n) {
    return (n >= 1 && n <= NUM_BADGES) ? (uint16_t)(FLAG_BADGE01_GET + (n - 1)) : 0;
}

int get_badge(SaveFile *save, int n, int *out) {
    uint16_t id = badge_flag_id(n);
    if (id == 0) return 0;
    return get_flag(save, id, out);
}

int set_badge(SaveFile *save, int n, int value) {
    uint16_t id = badge_flag_id(n);
    if (id == 0) return 0;
    return set_flag(save, id, value);
}

int get_badges(SaveFile *save, uint8_t *mask) {
    uint8_t result = 0;
    for (int n = 1; n <= NUM_BADGES; n++) {
        int bit;
        if (!get_badge(save, n, &bit)) return 0;
        if (bit) result |= (1 << (n - 1));
    }
    *mask = result;
    return 1;
}

int set_badges(SaveFile *save, uint8_t mask) {
    for (int n = 1; n <= NUM_BADGES; n++) {
        if (!set_badge(save, n, (mask >> (n - 1)) & 1)) return 0;
    }
    return 1;
}
