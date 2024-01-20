#ifndef GRID_H
#define GRID_H

#include "geom.h"
#include "error.h"
#include "event.h"
#include <MLV/MLV_all.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
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
    struct {
        void (*callback)(Point cell_pos, void* data);
        void* data;
    } click_handler;
    struct {
        void (*callback)(Point cell_pos, void* data);
        void* data;
    } hover_handler;
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
 * @brief Set grid's event handler on click
 * Called when a click event occurs on the grid
 * 
 * @param grid Grid object
 * @param callback Callback function taking the clicked cell's position
 * and a pointer to data.
 * @param data Pointer to data to pass to the callback function
 */
void Grid_set_on_click_handler(Grid* grid, void (*callback)(Point cell_pos, void* data), void* data);

/**
 * @brief Set grid's event handler on hover
 * 
 * @param grid Grid object
 * @param callback Callback function taking the hovered cell's position in the grid,
 * and the cursor's position in the window.
 * @param data Pointer to data to pass to the callback function
 */
void Grid_set_on_hover_handler(Grid* grid, void (*callback)(Point cell_pos, void* data), void* data);

/**
 * @brief Process grid event if any and call the callback function
 * Refreshes the hovered cell's color
 * 
 * @param grid 
 */
void Grid_process_event(Grid* grid);

/**
 * @brief Free grid
 * 
 * @param grid 
 */
void Grid_free(Grid* grid);

/**
 * @brief Get cell absolute position as a Rect
 * 
 * @param grid Grid object
 * @param cell_relative Cell coordinates relative to grid
 * @return Rect 
 */
Rect Grid_get_cell_rect(const Grid* grid, Point cell_relative);

/**
 * @brief Returns absolute coordinates of a cell's top-left corner
 * 
 * @param grid Grid object
 * @param cell_relative Cell coordinates relative to grid
 * @return Point 
 */
Point Grid_get_absolute_coords_TL(const Grid* grid, Point cell_relative);

/**
 * @brief Returns absolute coordinates of a cell's bottom-right corner
 * 
 * @param grid Grid object
 * @param cell_relative Cell coordinates relative to grid
 * @return Point 
 */
Point Grid_get_absolute_coords_BR(const Grid* grid, Point cell_relative);

/**
 * @brief Returns absolute coordinates of a cell's center
 * 
 * @param grid Grid object
 * @param cell_relative Cell coordinates relative to grid
 * @return Point 
 */
Point Grid_get_absolute_coords_C(const Grid* grid, Point cell_relative);

/**
 * @brief Returns cell's address in grid
 * 
 * @param grid Grid object
 * @param cell_relative Cell coordinates relative to grid 
 * @return Cell* 
 */
Cell* Grid_get_cell(const Grid* grid, Point cell_relative);

/**
 * @brief Draw only grid's lines
 * 
 * @param grid Grid object
 */
void Grid_draw_lines(const Grid* grid);

/**
 * @brief Draw grid with lines and per-cell colors
 * 
 * @param grid Grid object
 */
void Grid_draw_filled_rects(const Grid* grid);

/**
 * @brief Get the position on the grid
 * of absolute coordinates ``abs_pos``.
 * 
 * @param grid Grid object
 * @param abs_pos Absolute coordinates
 * @param rel_pos Address to store the
 * relative position
 * @return true if the point is inside the grid
 * @return false if the point is outside the grid
 */
bool Grid_absolute_pos_to_relative(const Grid* grid, Point abs_pos, Point* rel_pos);

#endif
