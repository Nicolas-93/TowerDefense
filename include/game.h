#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "land.h"
#include "inventory.h"
#include "buttons.h"
#include "counter.h"
#include "mana.h"

typedef struct Game {
    Grid viewport;
    Land land;
    Inventory inv;
    Buttons buttons;
    Counter counter;
    Mana mana;
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

/**
 * @brief Callback for when a gem is released
 * 
 * @param context Game object
 * @param object Gem object
 * @param abs_pos Release location
 * @return true 
 * @return false 
 */
bool Game_on_gem_release(
    void* context, void* object,
    Point abs_pos
);

/**
 * @brief Free all resources used by the Game
 * 
 * @param self 
 */
void Game_free(Game* self);

/**
 * @brief Check if the game is over
 * 
 * @param self Game object
 * @return true 
 * @return false 
 */
bool Game_is_game_over(const Game* self);

/**
 * @brief Draw the game over screen
 * 
 * @param self 
 */
void Game_draw_game_over(const Game* self);

#endif
