#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "inventory.h"

typedef struct Player {
    uint32_t mana;
    Inventory inv;
    uint32_t level;
} Player;

#endif
