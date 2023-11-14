#include "geom.h"
#include "utils.h"

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

Vector2D Vector2D_add(Vector2D v1, Vector2D v2) {
    return (Vector2D) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y
    };
}

Vector2D Vector2D_mul(Vector2D v, double n) {
    return (Vector2D) {
        .x = v.x * n,
        .y = v.y * n
    };
}


int Geom_manhattan_distance(Point a, Point b) {
    return ABS(a.x - b.x) + ABS(a.y - b.y);
}
