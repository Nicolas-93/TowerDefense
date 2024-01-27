#ifndef MANA_H
#define MANA_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "utils.h"
#include "grid.h"
#include "geom.h"

typedef struct Mana {
    uint32_t mana;
    uint32_t max_mana;
    uint32_t level;
    Rect rect;
    Grid* grid;
} Mana;

typedef enum ManaError {
    MANA_BANKRUPT           = -1,
    MANA_CAPACITY_REACHED   = -2,
} ManaError;

/**
 * @brief Create a new Mana object
 * 
 * @param self Mana object
 * @param grid parent grid
 * @param rect position of the mana bar in the parent grid
 * @param max_mana (should be 2000)
 * @param initial_mana (should be 150)
 * @return true if the player have mana
 * @return false if the player don't have mana
 */
Error Mana_new(Mana* self, Grid* parent, Rect rect, uint32_t initial_mana, uint32_t max_mana);

/**
 * @brief Add or remove mana to the player
 * 
 * @param self Mana object
 * @param amount if amount > 0, add mana, else remove mana
 * @return MANA_BANKRUPT if mana == 0 (end of the game)
 * @return MANA_CAPACITY_REACHED if some mana was waste
 * while adding (mana == max_mana)
 */
ManaError Mana_add(Mana* self, int amount);

/**
 * @brief Check if mana balance is sufficient to spend ``amount`` mana.
 * 
 * @return bool true if the player have sufficient mana
 * @return bool false if the player don't have sufficient mana
 */
bool Mana_have_sufficient_mana(const Mana* self, uint32_t amount);

/**
 * @brief Check if the player hava sufficient mana to spend ``amount`` mana.
 * 
 * @param self Mana object
 * @param amount amount of mana to spend
 * @return true Mana was spend
 * @return false Mana was not spend (not enough mana)
 */
bool Mana_buy(Mana* self, uint32_t amount);

/**
 * @brief Upgrade the mana level
 * 
 * @param self Mana object
 */
ManaError Mana_upgrade(Mana* self);

/**
 * @brief Get the cost of the mana pool upgrade
 * 
 * @param self Mana object
 * @return uint cost of the mana pool upgrade
 */
int Mana_get_pool_upgrade_cost(const Mana* self);

/**
 * @brief Get the cost of a gem
 * 
 * @param level level of the gem
 * @return int 
 */
int Mana_get_gem_cost(int level);

/**
 * @brief Get the cost of merging two gems
 * 
 * @return int 
 */
int Mana_get_gem_merging_cost(void);

/**
 * @brief Get bonus amount mana when a monster is killed
 * 
 * @param self Mana object
 * @param monster_initial_hp initial hp of the monster 
 * @return int 
 */
int Mana_get_killed_monster_bonus(const Mana* self, int monster_initial_hp);

/**
 * @brief Get the malus amount mana when a monster is back to spawn
 * 
 * @param self Mana object
 * @param monster_initial_hp initial hp of the monster
 * @return int 
 */
int Mana_get_monster_back_to_spawn_malus(const Mana* self, int monster_initial_hp);

/**
 * @brief Draw the mana bar, showing the current mana level
 * (current mana / max mana) as a progress bar.
 * And print the current and mana level.
 * 
 * @param self 
 */
void Mana_draw(const Mana* self);

#endif
