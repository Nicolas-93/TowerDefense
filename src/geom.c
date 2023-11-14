#include "geom.h"
#include "utils.h"

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
