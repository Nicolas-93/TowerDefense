#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "land.h"
#include "inventory.h"

typedef struct Game {
    Grid viewport;
    Land land;
    Inventory inv;
} Game;

/**
 * @brief Initialize a new Game object
 * 
 * @param self 
 * @return Error 
 */
Error Game_new(Game* self, Size window_size);

/**
 * @brief Update the Game
 * 
 * @param self 
 */
void Game_update(Game* self);

/**
 * @brief Draw the Game
 * 
 * @param self 
 */
void Game_draw(const Game* self);

/**
 * @brief Process an event
 * 
 * @param self 
 */
void Game_process_event(Game* self);


#endif
