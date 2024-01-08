#include "overlay.h"
#include <MLV/MLV_all.h>

void Overlay_draw(Point pos, const char *text, ...) {
    va_list args;
    va_start(args, text);

    va_list args_copy;
    va_copy(args_copy, args);
    int width = 0, height = 0;
    MLV_get_size_of_adapted_text_box_va(
        text,
        1,
        &width, &height,
        args_copy
    );
    va_end(args_copy);
    MLV_draw_adapted_text_box_va(
        pos.x - width / 2, pos.y - height,
        text,
        1,
        MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
        MLV_TEXT_CENTER,
        args
    );
    va_end(args);
}
