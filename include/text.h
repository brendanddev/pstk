#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>

// Converts one Gen 3 character code to ASCII. Unknown codes become '?'.
char gen3_to_ascii(uint8_t c);

#endif
