#include "counter.h"
#include "stdarg.h"
#include "utils.h"
#include <stdio.h>

static void _Counter_update_value(Counter* self, int diff) {
    self->value = clamp(self->value + diff, self->min, self->max);
}

static void _Counter_on_grid_click(Point pos, void* context) {
    Counter* self = (Counter*) context;
    if (pos.x == 0) {
        _Counter_update_value(self, -1);
    } else if (pos.x == 2) {
        _Counter_update_value(self, 1);
    }
}

Error Counter_new(Counter *self, Grid* parent, Rect rect, int min, int max) {
    *self = (Counter) {
        .min = min,
        .max = max,
        .value = min
    };

    if (Grid_new(
        &self->grid,
        3, 1, 0.8, parent, rect,
        true, MLV_COLOR_WHITE, MLV_COLOR_BLACK) < 0
    ) {
        return ERR_ALLOC;
    }
    Grid_set_on_click_handler(&self->grid, _Counter_on_grid_click, self);

    return 0;
}

void Counter_free(Counter *self) {
    Grid_free(&self->grid);
}

static void _Counter_draw_string(const Counter* self, Point pos, const char* str, ...) {
    pos = Grid_get_absolute_coords_TL(&self->grid, pos);
    va_list args;
    va_start(args, str);
    MLV_draw_text_box_va(
        pos.x, pos.y,
        self->grid.cell_width, self->grid.cell_height,
        str,
        1,
        MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
        MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER,
        args
    );
    va_end(args);
}

void Counter_draw(const Counter *self) {
    _Counter_draw_string(self, (Point) {.x = 0, .y = 0}, "-");
    _Counter_draw_string(self, (Point) {.x = 1, .y = 0}, "%d", self->value);
    _Counter_draw_string(self, (Point) {.x = 2, .y = 0}, "+");
    Grid_draw_lines(&self->grid);
}

void Counter_process_event(Counter* self) {
    Grid_process_event(&self->grid);
}
