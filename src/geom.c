#include "geom.h"


Vector2D Vector2D_add(Vector2D v1, Vector2D v2) {
    return (Vector2D) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y
    };
}