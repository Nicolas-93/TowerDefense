#ifndef TOWER_H
#define TOWER_H

#include "gem.h"
#include "timer.h"
#include "grid.h"
#include "monster.h"
#include "deque.h"

typedef struct Tower {
    Point pos;              /*< Position on the grid */
    const Grid* grid;       /*< Land's grid */
    bool is_empty;          /*< True if the tower has no gem */
    bool is_active;         /*< True if the tower is active (tower is sleeping 2s after a gem is placed) */
    Timer active_timer;     /*< 2 seconds to wait before the tower begin to shoot */
    Timer shoot_timer;      /*< 0.5 seconds between each shot */
    Gem gem;
    float range;
} Tower;

/**
 * @brief Create a new Tower object, without gem.
 * 
 * @param self 
 * @param grid Land's grid
 * @param pos Position of the tower on the grid
 * @return Tower 
 */
void Tower_new(Tower* self, const Grid* grid, Point pos);

/**
 * @brief Attack the monsters with the towers.
 * 
 * @param self 
 * @return Error 
 */
Error Tower_anim(Tower* self, Deque* monsters);

/**
 * @brief Put a gem in a tower, and start a timer
 * of 2 seconds, before the tower becomes active.
 * 
 * @param self 
 * @param gem 
 */
void Tower_set_gem(Tower* self, Gem gem);

/**
 * @brief Draw the tower on the grid.
 * 
 * @param self 
 */
void Tower_draw(const Tower* self);

#endif
