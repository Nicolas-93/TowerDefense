#include "traject.h"
#include <MLV/MLV_time.h>


Traject Traject_new(Point pos, Vector2D dir, float base_unit) {
    double norm = Vector2D_normalize(&dir);
    Traject self = (Traject) {
        .pos = pos,
        .dir = dir,
        .base_unit = base_unit,
        .speed_by_unit = norm,
    };
    Traject_set_speed(&self, norm);

    return self;
}

Traject Traject_new_without_dir_and_pos(float base_unit, float speed_by_unit) {
    Traject self = (Traject) {
        .pos = (Point) {0, 0},
        .dir = (Vector2D) {0, 0},
        .base_unit = base_unit,
        .speed_by_unit = speed_by_unit,
    };
    Traject_set_speed(&self, speed_by_unit);

    return self;
}

Traject Traject_new_from_points(Point a, Point b, float base_unit, float speed_by_unit) {
    Traject self = (Traject) {
        .pos = a,
        .base_unit = base_unit,
    };
    Traject_pursuit(&self, b);
    Traject_set_speed(&self, speed_by_unit);

    return self;
}

void Traject_pursuit(Traject* self, Point target) {
    Vector2D AB = Vector2D_sub(target, self->pos);
    Vector2D_normalize(&AB);
    self->dir = AB;
}

void Traject_set_from_points(Traject* self, Point a, Point b) {
    self->pos = a;
    Traject_pursuit(self, b);
}

void Traject_move(Traject* self) {
    self->pos = Vector2D_add(
        self->pos,
        Vector2D_mul(self->dir, self->speed_by_frame)
    );
}

bool Traject_is_over(const Traject* self, Point target) {
    return Vector2D_dist(self->pos, target) < self->speed_by_frame;
}

void Traject_set_speed(Traject* self, float speed_by_unit) {
    self->speed_by_unit = speed_by_unit;
    self->speed_by_frame = speed_by_unit * (self->base_unit / MLV_get_frame_rate());
}

float Traject_get_speed(const Traject* self) {
    return self->speed_by_unit;
}

float Traject_get_base_unit(const Traject* self) {
    return self->base_unit;
}
