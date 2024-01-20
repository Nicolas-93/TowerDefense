#ifndef MANA_H
#define MANA_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "utils.h"

typedef struct Mana {
    uint32_t mana;
    uint32_t max_mana;
    uint32_t level;
} Mana;

typedef enum ManaError {
    MANA_BANKRUPT           = -1,
    MANA_CAPACITY_REACHED   = -2,
} ManaError;

/**
 * @brief Create a new Mana object
 * 
 * @param self Mana object
 * @param max_mana (should be 2000)
 * @param initial_mana (should be 150)
 * @return true if the player have mana
 * @return false if the player don't have mana
 */
bool Mana_new(Mana* self, uint32_t initial_mana, uint32_t max_mana);

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
 * @brief Upgrade the mana level
 * 
 * @param self Mana object
 * @return true if the player have enough mana
 * @return false if the player don't have enough mana
 */
bool Mana_upgrade(Mana* self);

#endif
