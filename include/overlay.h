#ifndef OVERLAY_H
#define OVERLAY_H

#include "geom.h"
#include <stdarg.h>

__attribute__ ((format (printf, 2, 3)))
void Overlay_draw(Point pos, const char *text, ...);

#endif
