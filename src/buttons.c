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
    if (button->callback.on_click != NULL) {
        button->callback.on_click(button->callback.context);
    }
}

static void _Buttons_on_grid_hover(Point cell_pos, void* context) {
    Buttons* self = (Buttons*) context;
    int i = _Buttons_get_button_index(self, cell_pos);

    if (i >= self->nb_buttons)
        return;
    
    Button* button = &self->buttons[i];
    button->_is_hovered = true;
}

Error Buttons_new(Buttons* self, Grid* parent, Rect rect, Size grid_size, Button buttons[], int nb_buttons) {
    *self = (Buttons) {
        .buttons = buttons,
        .nb_buttons = nb_buttons,
    };

    if (Grid_new(
        &self->grid,
        grid_size.width, grid_size.height, 0.8, parent, rect,
        true, MLV_COLOR_WHITE, MLV_COLOR_BLACK) < 0
    ) {
        return ERR_ALLOC;
    }
    Grid_set_on_click_handler(&self->grid, _Buttons_on_grid_click, self);
    Grid_set_on_hover_handler(&self->grid, _Buttons_on_grid_hover, self);

    return 0;
}

void Button_draw(const Button* self, const Buttons* buttons, Point pos) {
    Point TL_pos = Grid_get_absolute_coords_TL(&buttons->grid, pos);
    MLV_Image* icon = Image_get(
        self->icon,
        (Size) {.width = buttons->grid.cell_width, .height = buttons->grid.cell_height}
    );
    MLV_draw_image(icon, TL_pos.x, TL_pos.y);
}

/**
 * @brief Call the on_hover callback of the hovered button
 * 
 * @param self 
 */
static void _Buttons_call_hover_callback(const Buttons* self) {
    for (int i = 0; i < self->nb_buttons; i++) {
        Button* button = &self->buttons[i];
        // Point pos = _Buttons_get_button_pos(self, i);

        if (button->callback.on_hover != NULL && button->_is_hovered) {
            button->callback.on_hover(
                Event_get().mouse,
                // Grid_get_absolute_coords_C(&self->grid, pos),
                button->callback.context
            );
        }
    }
}

void Buttons_draw(const Buttons* self) {
    Grid_draw_lines(&self->grid);

    for (int i = 0; i < self->nb_buttons; i++) {
        Point pos = _Buttons_get_button_pos(self, i);
        Button_draw(&self->buttons[i], self, pos);
    }
    _Buttons_call_hover_callback(self);
}

/**
 * @brief Reset hovered state of all buttons
 * 
 * @param self 
 */
static void _Buttons_reset_hovered(Buttons* self) {
    for (int i = 0; i < self->nb_buttons; i++) {
        self->buttons[i]._is_hovered = false;
    }
}

void Buttons_process_event(Buttons* self) {
    _Buttons_reset_hovered(self);
    Grid_process_event(&self->grid);
}

void Buttons_free(Buttons* self) {
    Grid_free(&self->grid);
    *self = (Buttons) {0};
}
