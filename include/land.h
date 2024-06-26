#ifndef LAND_H
#define LAND_H

#include "error.h"
#include "grid.h"
#include "tower.h"
#include "deque.h"
#include "monster.h"
#include "monsterwave.h"
#include "path.h"
#include "timer.h"
#include "event.h"
#include "mana.h"

#define H_PARAMETER 25

typedef struct Game Game;

typedef struct Land {
    Path path;              /*< Monsters path */
    Deque towers;           /*< Placed towers */
    int available_towers;   /*< Avalaible towers to place*/
    Deque monsters;         /*< Monsters on the land */
    uint16_t wave_counter;  /*< Monsters waves counter */
    Grid grid;              /*< Land's grid */
    Timer next_wave_timer;  /*< Time before next wave */
    Game* game;             /*< Parent game context, used for dragndrop */
} Land;

/**
 * @brief Create a new Land object, with a given width and height.
 * 
 * @param self 
 * @param parent Parent grid
 * @param rectn Position of the land on the parent grid
 * @param game Parent game object
 * @param width 
 * @param height 
 * @return Error 
 */
Error Land_new(Land* self, Grid* parent, Rect rect, Game* game, uint16_t width, uint16_t height);

/**
 * @brief Add a tower to the land
 * 
 * @param self 
 * @param tower 
 * @return Error ERR_TOWER_ALREADY_PRESENT if a tower is already present
 */
Error Land_add_tower(Land* self, Tower* tower);

/**
 * @brief Add a wave of monsters to the land
 * 
 * @param self 
 * @param wave Type of the wave
 */
void Land_new_monster_wave(Land* self, MonsterWave wave);

/**
 * @brief Add a random wave of monsters to the land
 * 
 * @param self 
 */
void Land_new_random_monster_wave(Land* self);

/**
 * @brief Check if a cell is part of the path
 * 
 * @param self 
 * @param p Position to test
 * @return true 
 * @return false 
 */
bool Land_is_path(const Land* self, Point p);

/**
 * @brief Check if a cell is part of a tower
 * 
 * @param self 
 * @param p Position to test
 * @return true 
 * @return false 
 */
bool Land_is_tower(const Land* self, Point p);

/**
 * @brief Check if a cell is occupied by a tower or the path
 * 
 * @param self 
 * @param p Position to test
 * @return true 
 * @return false 
 */
bool Land_is_occupied(const Land* self, Point p);

/**
 * @brief Process and move land's objects
 * 
 * @param self 
 */
void Land_update(Land* self);

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

/**
 * @brief Process land's events (clicks on the grid)
 * 
 * @param self 
 */
void Land_process_event(Land* self);

/**
 * @brief Callback for when a gem is released, used
 * by dragndrop
 * 
 * @param context Land object
 * @param object Gem object
 * @param abs_pos Absolute position of the gem
 * @return true
 * @return false
 */
bool Land_on_gem_release(void* context, void* object, Point abs_pos);

#endif
