#ifndef LAND_H
#define LAND_H

#include "error.h"
#include "grid.h"
#include "tower.h"
#include "deque.h"
#include "monster.h"
#include "path.h"
#include "timer.h"

#define H_PARAMETER 1

typedef struct Land {
    Path path;              /*< Monsters path */
    Deque towers;           /*< Placed towers */
    Deque monsters;         /*< Monsters on the land */
    uint16_t wave_counter;  /*< Monsters waves counter */
    Grid grid;              /*< Land's grid */
    Timer next_wave_timer;  /*< Time before next wave */
} Land;

typedef enum MonsterWaveType {
    WAVE_NORMAL,
    WAVE_CROWD,
    WAVE_AGILE,
    WAVE_BOSS
} MonsterWaveType;

typedef struct MonsterWave {
    MonsterWaveType type;
    int probability;
    int size;
    int speed;
} MonsterWave;

#define NB_MONSTER_WAVES 4

extern const MonsterWave WAVES[NB_MONSTER_WAVES];

#define WAVES_PROBABILITIES { \
    [WAVE_NORMAL] = 50, \
    [WAVE_CROWD] = 20, \
    [WAVE_AGILE] = 20, \
    [WAVE_BOSS] = 10, \
}

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
