#ifndef GRID_H
#define GRID_H

#include "geom.h"
#include "error.h"
#include <MLV/MLV_all.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    Rect pos;
    MLV_Color filled_color;
    void* data;
} Cell;

typedef struct {
    uint16_t width;
    uint16_t height;
    Rect view;
    float cell_width;
    float cell_height;
    float margin;
    bool square;
    MLV_Color border_color;
    MLV_Color filled_color;
    Cell** cells;
} Grid;

/**
 * @brief Creates a grid of the specified number of columns and rows,
 * positioned according to ``grid_position`` at absolute positions of the window if
 * ``grid_base`` is NULL, otherwise relative to ``grid_base``.
 * with respect to the requested coordinates,
 *
 * @param self Grid to initialize
 * @param width Number of columns
 * @param height Number of rows
 * @param margin Coefficient of margin
 * @param grid_base Optional, reference grid.
 * @param grid_position Absolute position of the grid in the window,
 * or position relative to ``grid_base``.
 * @param square Specifies whether the grid should be square or stretched
 * @param filled_color Color of cell background
 * @param color_border Color of cell borders
 * @return Error
 */
Error Grid_new(
    Grid* self,
    uint16_t width, uint16_t height,
    float margin,
    Grid* grid_base, Rect grid_position, bool square,
    MLV_Color filled_color, MLV_Color color_border);

/**
 * @brief Free grid
 * 
 * @param grid 
 */
void Grid_free(Grid* grid);

/**
 * @brief Returns absolute coordinates of a cell's top-left corner
 * 
 * @param grid 
 * @param cell_relative 
 * @return Point 
 */
Point Grid_get_absolute_coords_TL(Grid* grid, Point cell_relative);


/**
 * @brief Returns absolute coordinates of a cell's bottom-right corner
 * 
 * @param grid 
 * @param cell_relative 
 * @return Point 
 */
Point Grid_get_absolute_coords_BR(Grid* grid, Point cell_relative);

/**
 * @brief Draw only grid's lines
 * 
 * @param grid 
 */
void Grid_draw_lines(const Grid* grid);

/**
 * @brief Draw grid with lines and per-cell colors
 * 
 * @param grid 
 */
void Grid_draw_filled_rects(const Grid* grid);

#endif
