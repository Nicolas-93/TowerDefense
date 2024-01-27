#ifndef GFXUTILS_H
#define GFXUTILS_H

#include <MLV/MLV_all.h>
#include "geom.h"

/**
 * @brief Draw a completion bar
 * 
 * @param rect Rectangle to draw in
 * @param completion Completion percentage (between 0 and 1)
 * @param color_background Background color
 * @param color_completion Completion color (completed part)
 */
void Gfx_draw_completion_bar(
    Rect rect,
    float completion,
    MLV_Color color_background,
    MLV_Color color_completion
);

#endif
