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
 * @brief Create a new allocated gem
 * 
 * @param self 
 * @param grid grid
 * @param level Gem's level
 * @return Gem* allocated gem
 */
Gem* Gem_new(Grid* grid, int level);

/**
 * @brief Check if two gems are mergeable
 * (same level)
 * 
 * @param self 
 * @param other 
 * @return true 
 * @return false 
 */
bool Gem_is_mergeable(const Gem* self, const Gem* other);

/**
 * @brief Merge ``other`` into ``self`` and free ``other``
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

/**
 * @brief Free ressources used by the gem
 * 
 * @param self 
 */
void Gem_free(Gem* self);

#endif
