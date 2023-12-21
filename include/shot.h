#ifndef SHOT_H
#define SHOT_H

#include "geom.h"
#include "gem.h"
#include "grid.h"
#include "traject.h"
#include <MLV/MLV_color.h>

typedef struct Shot {
    Traject traj;
    Point* target;
    Gem gem;
} Shot;

/**
 * @brief Create a new Shot object.
 * 
 * @param self Shot to create
 * @param grid Land's grid.
 * @param start Starting point of the shot.
 * @param target Target monster to pursue.
 * @param gem Gem that made the shot.
 */
void Shot_new(
    Shot* self,
    const Grid* grid,
    Point start,
    Point* target,
    Gem gem
);

/**
 * @brief Draw the shot.
 * 
 * @param shot Shot to draw.
 */
void Shot_draw(const Shot* shot);

/**
 * @brief Check if the shot has reached its target.
 * 
 * @param shot Shot to check.
 * @return true 
 * @return false 
 */
bool Shot_has_reached_target(const Shot* shot);

/**
 * @brief Update shot's position.
 * 
 * @param shot Shot to update.
 */
void Shot_update(Shot* shot);

#endif
