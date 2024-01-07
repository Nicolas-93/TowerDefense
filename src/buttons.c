#include "buttons.h"
#include <assert.h>

static inline Point _Buttons_get_button_pos(const Buttons* self, int i) {
    return (Point) {
        .x = i % self->grid.width,
        .y = i / (self->grid.height + 1),
    };
}

static inline int _Buttons_get_button_index(const Buttons* self, Point pos) {
    return pos.x + pos.y * self->grid.width;
}

static void _Buttons_on_grid_click(Point pos, void* context) {
    Buttons* self = (Buttons*) context;
    int i = _Buttons_get_button_index(self, pos);

    if (i >= self->nb_buttons)
        return;

    Button* button = &self->buttons[i];
    if (button->callback.func != NULL) {
        button->callback.func(button->callback.context);
    }
}

Error Buttons_new(Buttons* self, Grid* parent, Rect rect, Button buttons[], int nb_buttons) {
    *self = (Buttons) {
        .buttons = buttons,
        .nb_buttons = nb_buttons,
    };

    if (Grid_new(&self->grid, 3, 2, 0.8, parent, rect, true, MLV_COLOR_WHITE, MLV_COLOR_BLACK) < 0) {
        return ERR_ALLOC;
    }
    Grid_set_event_handler(&self->grid, _Buttons_on_grid_click, self);

    return 0;
}

void Button_draw(const Button* self, const Buttons* buttons, Point pos) {
    pos = Grid_get_absolute_coords_TL(&buttons->grid, pos);
    MLV_Image* icon = Image_get(
        self->icon,
        (Size) {.width = buttons->grid.cell_width, .height = buttons->grid.cell_height}
    );
    MLV_draw_image(icon, pos.x, pos.y);
}

void Buttons_draw(const Buttons* self) {
    Grid_draw_lines(&self->grid);

    for (int i = 0; i < self->nb_buttons; i++) {
        Point pos = _Buttons_get_button_pos(self, i);
        Button_draw(&self->buttons[i], self, pos);
    }
}

void Buttons_process_event(Buttons* self) {
    Grid_process_event(&self->grid);
}
