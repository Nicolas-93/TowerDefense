#ifndef LAND_H
#define LAND_H

#include "error.h"
#include "grid.h"
#include "tower.h"
#include "deque.h"
#include "monster.h"
#include "path.h"

typedef struct Land {
    Path path;              /*< Monsters path */
    Deque towers;           /*< Placed towers */
    Deque monsters;         /*< Monsters on the land */
    uint16_t wave_counter;  /*< Monsters waves counter */
    Grid grid;              /*< Land's grid */
} Land;

/**
 * @brief Create a new Land object, with a given width and height.
 * 
 * @param self 
 * @param parent Parent grid
 * @param width 
 * @param height 
 * @return Error 
 */
Error Land_new(Land* self, Grid* parent, uint16_t width, uint16_t height);

/**
 * @brief Add a tower to the land
 * 
 * @param self 
 * @param tower 
 */
void Land_add_tower(Land* self, Tower* tower);

/**
 * @brief Add a monster to the land
 * 
 * @param self 
 * @param monster 
 */
void _Land_add_monster(Land* self, Monster* monster);

/**
 * @brief Add a wave of monsters to the land
 * 
 * @param self 
 */
void Land_add_monster_wave(Land* self); 

/**
 * @brief Check if a cell is part of the path
 * 
 * @param self 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool Land_is_path(const Land* self, const uint16_t x, const uint16_t y);

/**
 * @brief Check if a cell is part of a tower
 * 
 * @param self 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool Land_is_tower(const Land* self, const uint16_t x, const uint16_t y);

/**
 * @brief Check if a cell is occupied by a tower or the path
 * 
 * @param self 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool Land_is_occupied(const Land* self, const uint16_t x, const uint16_t y);

/**
 * @brief Process and move land's objects
 * 
 * @param self 
 */
void Land_anim(Land* self);

/**
 * @brief Draw the land
 * 
 * @param self 
 */
void Land_draw(const Land* self);

/**
 * @brief Free the land
 * 
 * @param self 
 */
void Land_free(Land* self);

#endif
