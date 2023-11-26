#include "shot.h"

void Shot_new(
    Shot* self,
    const Grid* grid,
    Point start,
    Point* target,
    Gem gem
) {
    *self = (Shot) {
        .traj = Traject_new_from_points(
            start, *target,
            grid->cell_width, 3
        ),
        .target = target,
        .gem = gem,
    };
}

void Shot_draw(const Shot* shot) {
    Color color = shot->gem.color;
    MLV_draw_filled_circle(
        shot->traj.pos.x, shot->traj.pos.y,
        3, color.mlvrgb | MLV_COLOR_BLACK
    );
}

bool Shot_has_reached_target(const Shot* shot) {
    return Traject_is_over(&shot->traj, *shot->target);
}

void Shot_anim(Shot* self) {
    Traject_pursuit(&self->traj, *self->target);
    Traject_move(&self->traj);
}
