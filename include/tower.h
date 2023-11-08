#ifndef TOWER_H
#define TOWER_H

#include "gem.h"
#include "timer.h"
#include "grid.h"
#include "monster.h"

typedef struct Tower {
    Point pos;
    Grid* grid;
    bool is_empty;
    bool is_active;
    Timer active_timer;     /*< 2 seconds to wait before the tower begin to shoot */
    Timer shoot_timer;      /*< 0.5 seconds between each shot */
    Gem gem;
} Tower;

/**
 * @brief Create a new Tower object, without gem.
 * 
 * @param self 
 * @param grid Land's grid
 * @param pos Position of the tower on the grid
 * @return Tower 
 */
void Tower_new(Tower* self, Grid* grid, Point pos);

/**
 * @brief Put a gem in a tower, and start a timer
 * of 2 seconds, before the tower becomes active.
 * 
 * @param self 
 * @param gem 
 */
void Tower_set_gem(Tower* self, Gem gem);

/**
 * @brief Attack the monsters with the towers.
 * Each tower shoot a monster in its range, having the lowest HP.
 * Do nothing if the tower is empty, or the shoot timer isn't completed.
 * 
 * @param self 
 */
static void Tower_attack(Tower* self, Deque* monsters);

#endif
