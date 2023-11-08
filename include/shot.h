#ifndef SHOT_H
#define SHOT_H

#include "geom.h"
#include "gem.h"
#include "traject.h"
#include <MLV/MLV_color.h>

typedef struct Shot {
    Traject traj;
    Gem gem;
} Shot;

void Shot_draw(const Shot* shot);

void Shot_anim(Shot* shot);

#endif
