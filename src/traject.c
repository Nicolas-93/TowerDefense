#include "traject.h"

Traject Traject_new(Point pos, Vector2D dir) {
    double norm = Vector2D_normalize(&dir);
    return (Traject) {
        .pos = pos,
        .dir = dir,
        .speed = norm,
    };
}

Traject Traject_new_from_points(Point a, Point b, float speed) {
    Vector2D AB = Vector2D_sub(b, a);
    Vector2D_normalize(&AB);
    return (Traject) {
        .pos = a,
        .dir = AB,
        .speed = speed,
    };
}

void Traject_move(Traject* self) {
    self->pos = Vector2D_add(
        self->pos,
        Vector2D_mul(self->dir, self->speed)
    );
}

bool Traject_is_over(Traject* self, Point target) {
    return Vector2D_dist(self->pos, target) < self->speed;
}
