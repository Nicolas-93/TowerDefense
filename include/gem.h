#ifndef GEM_H
#define GEM_H

#include "color.h"
#include "grid.h"

typedef enum GemType {
    PYRO = 1,
    DENDRO = 2,
    HYDRO = 3,
    MIXED = 4,
} GemType;

typedef struct Gem {
    Grid* grid;
    Color color;
    GemType type;
    uint8_t level;
} Gem;

/**
 * @brief Create a new gem
 * 
 * @param self 
 * @param grid grid
 * @param level Gem's level
 */
void Gem_new(Gem* self, Grid* grid, int level);

/**
 * @brief Merge ``other`` into ``self``
 * 
 * @param self Merged gem
 * @param other Gem being merged into ``self`` 
 */
void Gem_merge(Gem* self, Gem* other);

/**
 * @brief Draw a gem while dragging it
 * 
 * @param self Gem object
 * @param pos_abs Absolute position of the mouse
 */
void Gem_draw_dragndrop(void* self, Point pos_abs);

/**
 * @brief Draw a gem on the grid
 * 
 * @param self Gem object
 * @param pos_rel Relative position of the gem on the grid
 */
void Gem_draw_grid(Gem* self, Point pos_rel);

/**
 * @brief Set the grid of the gem
 * 
 * @param self Gem object
 * @param grid Grid in which the gem is placed
 */
void Gem_set_grid(Gem* self, Grid* grid);

#endif
