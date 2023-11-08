#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "inventory.h"
#include "mana.h"

typedef struct Player {
    Mana mana;
    Inventory inv;
} Player;

/**
 * @brief Create a new Player object
 * 
 * @param self Player object
 */
void Player_new(Player* self);

bool Player_buy_gem(Player* self);

bool Player_buy_mana_upgrade(Player* self);

bool Player_buy_tower(Player* self);

#endif
