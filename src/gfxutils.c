#include "gfxutils.h"

void Gfx_draw_completion_bar(
    Rect rect,
    float completion,
    MLV_Color color_background,
    MLV_Color color_completion
) {
    // background
    MLV_draw_filled_rectangle(
        rect.a.x, rect.a.y,
        Rect_get_width(rect),
        Rect_get_height(rect),
        color_background
    );

    // Draw completed
    MLV_draw_filled_rectangle(
        rect.a.x, rect.a.y,
        Rect_get_width(rect) * completion,
        Rect_get_height(rect),
        color_completion
    );
}
