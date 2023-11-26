#ifndef GEM_H
#define GEM_H

#include "color.h"

typedef enum GemType {
    PYRO = 1,
    DENDRO = 2,
    HYDRO = 3,
    MIXED = 4,
} GemType;

typedef struct Gem {
    Color color;
    GemType type;
    uint8_t level;
} Gem;


#endif
