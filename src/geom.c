#include "geom.h"
#include "utils.h"
#include <math.h>

const char* DIRECTIONS_STR[4] = {
    [UP]    = "UP",
    [DOWN]  = "DOWN",
    [RIGHT] = "RIGHT",
    [LEFT]  = "LEFT",
};

const Vector2D DIRECTIONS_VECT[4] = {
    [LEFT]  = {-1,  0},
    [RIGHT] = { 1,  0}, 
    [UP]    = { 0, -1},
    [DOWN]  = { 0,  1},
};

const Axis DIRECTION_TO_AXIS[4] = {
    [LEFT]  = HORIZONTAL,
    [RIGHT] = HORIZONTAL,
    [UP]    = VERTICAL,
    [DOWN]  = VERTICAL,
};

Vector2D Vector2D_new(double x, double y) {
    return (Vector2D) {
        .x = x,
        .y = y
    };
}

Vector2D Vector2D_new_from_value(double value) {
    return (Vector2D) {
        .x = value,
        .y = value
    };
}

bool Vector2D_equals(Vector2D v1, Vector2D v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

Vector2D Vector2D_add(Vector2D v1, Vector2D v2) {
    return (Vector2D) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y
    };
}

Vector2D Vector2D_sub(Vector2D v1, Vector2D v2) {
    return (Vector2D) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y
    };
}

Vector2D Vector2D_mul(Vector2D v, double n) {
    return (Vector2D) {
        .x = v.x * n,
        .y = v.y * n
    };
}

double Vector2D_norm(Vector2D v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

double Vector2D_normalize(Vector2D* v) {
    double norm = Vector2D_norm(*v);
    if (norm != 0) {
        v->x /= norm;
        v->y /= norm;
    }
    return norm;
}

double Vector2D_dist(Point a, Point b) {
    return Vector2D_norm(Vector2D_sub(b, a));
}

int Geom_manhattan_distance(Point a, Point b) {
    return ABS(a.x - b.x) + ABS(a.y - b.y);
}

bool Point_on_segment(Point cell, Point p1, Point p2) {
    if (p1.x == p2.x) {
        return (
            cell.x == p1.x && cell.y >= MIN(p1.y, p2.y)
                           && cell.y <= MAX(p1.y, p2.y)
        );
    }
    if (p1.y == p2.y) {
        return (
            cell.y == p1.y && cell.x >= MIN(p1.x, p2.x)
                           && cell.x <= MAX(p1.x, p2.x)
        );
    }
    return false;
}

bool Point_on_circle(Point pos, Point center, double radius) {
    return Vector2D_norm(Vector2D_sub(pos, center)) <= radius;
}

bool Rect_contains(Rect rect, Point p) {
    return (
        p.x >= rect.ax && p.x < rect.bx
        && p.y >= rect.ay && p.y < rect.by
    );
}
