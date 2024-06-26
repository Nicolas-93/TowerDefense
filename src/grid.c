#include "grid.h"
#include "utils.h"
#include <MLV/MLV_all.h>
#include <assert.h>

static int _Grid_init_cells(Grid* grid);

Error Grid_new(
    Grid* self,
    uint16_t width, uint16_t height,
    float margin,
    Grid* grid_base, Rect grid_position, bool square,
    MLV_Color filled_color, MLV_Color color_border) {

    *self = (Grid) {
        .width = width, .height = height,
        .margin = margin,
        .square = square,
        .filled_color = filled_color, .border_color = color_border
    };

    if (grid_base != NULL) {
        self->view.a = Grid_get_absolute_coords_TL(
            grid_base,
            grid_position.a
        );
        self->view.b = Grid_get_absolute_coords_BR(
            grid_base,
            grid_position.b
        );
    }
    else {
        self->view = grid_position;
    }

    if (_Grid_init_cells(self) == ERR_ALLOC) {
        return ERR_ALLOC;
    }

    return 0;
}

void Grid_set_on_click_handler(Grid* grid, void (*callback)(Point cell_pos, void* data), void* data) {
    assert(callback != NULL);
    grid->click_handler.data = data;
    grid->click_handler.callback = callback;
}

void Grid_set_on_hover_handler(Grid* grid, void (*callback)(Point cell_pos, void* data), void* data) {
    assert(callback != NULL);
    grid->hover_handler.data = data;
    grid->hover_handler.callback = callback;
}

/**
 * @brief Convert absolute coordinates to relative grid's coordinates
 * 
 * @param grid Grid object
 * @param absolute Absolute coordinates (usually mouse position)
 * @return Point 
 */
static inline Point _Grid_absolute_to_relative(const Grid* grid, Point absolute) {
    return (Point) {
        .x = (int) ((absolute.x - grid->view.ax) / grid->cell_width),
        .y = (int) ((absolute.y - grid->view.ay) / grid->cell_height)
    };
}

bool Grid_absolute_pos_to_relative(const Grid* grid, Point abs_pos, Point* rel_pos) {

    if (!Rect_contains(grid->view, abs_pos)) {
        return false;
    }

    *rel_pos = _Grid_absolute_to_relative(grid, abs_pos);
    return true;
}

/**
 * @brief Process click event if any and call the callback function
 * 
 * @param grid 
 */
static void _Grid_process_event(Grid* grid) {
    if (!grid->hover_handler.callback && !grid->click_handler.callback)
        return;

    Point pos;
    Event ev = Event_get();
    if (!Grid_absolute_pos_to_relative(grid, ev.mouse, &pos))
        return;

    if (Event_is_pressed_click() && grid->click_handler.callback)
        grid->click_handler.callback(pos, grid->click_handler.data);
    
    if (grid->hover_handler.callback)
        grid->hover_handler.callback(pos, grid->hover_handler.data);
}

void Grid_process_event(Grid* grid) {
    _Grid_process_event(grid);
}

void Grid_free(Grid* grid) {
    for (int i = 0; i < grid->height; ++i) {
        free(grid->cells[i]);
    }
    free(grid->cells);
}

Rect Grid_get_cell_rect(const Grid* grid, Point cell_relative) {
    assert(Rect_contains((Rect) {
            .a = {0, 0},
            .b = {grid->width, grid->height}
        },
        cell_relative
    ));
    return (Rect) {
        .a = {
            .x = grid->view.ax + cell_relative.x * grid->cell_width,
            .y = grid->view.ay + cell_relative.y * grid->cell_height
        },
        .b = {
            .x = grid->view.ax + (cell_relative.x + 1) * grid->cell_width,
            .y = grid->view.ay + (cell_relative.y + 1) * grid->cell_height
        }
    };
}

Point Grid_get_absolute_coords_TL(const Grid* grid, Point cell_relative) {
    return Grid_get_cell_rect(grid, cell_relative).a;
}

Point Grid_get_absolute_coords_BR(const Grid* grid, Point cell_relative) {
    return Grid_get_cell_rect(grid, cell_relative).b;
}

Point Grid_get_absolute_coords_C(const Grid* grid, Point cell_relative) {
    Point TL = Grid_get_absolute_coords_TL(grid, cell_relative);
    return (Point) {
        .x = TL.x + grid->cell_width / 2,
        .y = TL.y + grid->cell_height / 2
    };
}

static Error _Grid_init_cells(Grid* grid) {
    Cell** cells = calloc(grid->height, sizeof(Cell*));
    if (cells == NULL) {
        return ERR_ALLOC;
    }

    int outside_width  = Rect_get_width(grid->view);
    int outside_height = Rect_get_height(grid->view);

    int inside_width  = outside_width  * grid->margin;
    int inside_height = outside_height * grid->margin;

    grid->cell_width  = inside_width  / grid->width;
    grid->cell_height = inside_height / grid->height;

    if (grid->square) {
        grid->cell_width = grid->cell_height = MIN(grid->cell_width, grid->cell_height);
    }

    grid->view.ax += (outside_width
                        - grid->cell_width * grid->width) / 2;
    grid->view.ay += (outside_height
                        - grid->cell_height * grid->height) / 2;
    
    grid->view.bx = grid->view.ax + grid->width * grid->cell_width;
    grid->view.by = grid->view.ay + grid->height * grid->cell_height;

    for (int j = 0; j < grid->height; ++j) {
        Cell* ligne = calloc(grid->width, sizeof(Cell));
        if (cells == NULL) {
            free(cells);
            return 0;
        }
        cells[j] = ligne;
        for (int i = 0; i < grid->width; ++i) {
            cells[j][i] = (Cell) {
                .filled_color = grid->filled_color
            };
        }
    }
    grid->cells = cells;

    return 0;
}

void Grid_draw_lines(const Grid* grid) {
    for (int i = 0; i <= grid->width; ++i) {
        MLV_draw_line(
            grid->view.ax + (i * grid->cell_width), grid->view.ay,
            grid->view.ax + (i * grid->cell_width), grid->view.by,
            grid->border_color
        );
    }

    for (int j = 0; j <= grid->height; ++j) {
        MLV_draw_line(
            grid->view.ax, grid->view.ay + (j * grid->cell_height),
            grid->view.bx, grid->view.ay + (j * grid->cell_height),
            grid->border_color
        );
    }
}

void Grid_draw_filled_rects(const Grid* grid) {
    for (int i = 0; i < grid->height; ++i) {
        for (int j = 0; j < grid->width; ++j) {
            Rect cell_rect = Grid_get_cell_rect(grid, (Point) {.x = j, .y = i});
            MLV_draw_filled_rectangle(
                cell_rect.ax, cell_rect.ay,
                grid->cell_width, grid->cell_height,
                grid->cells[i][j].filled_color
            );
        }
    }
}

Cell* Grid_get_cell(const Grid* grid, Point cell_relative) {
    if (!Rect_contains((Rect) {
            .a = {0, 0},
            .b = {grid->width, grid->height}
        },
        cell_relative
    )) {
        return NULL;
    }

    return &grid->cells[(int) cell_relative.y][(int) cell_relative.x];
}
