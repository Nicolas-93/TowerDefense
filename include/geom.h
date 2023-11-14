#ifndef GEOM_H
#define GEOM_H

typedef struct {
    double x;
    double y;
} Vector2D, Point;

typedef union {
    struct {
        Point a;
        Point b;
    };
    struct {
        double ax;
        double ay;
        double bx;
        double by;
    };
} Rect;

typedef enum Direction {
    UP = 0,
    DOWN,
    RIGHT,
    LEFT,
} Direction;

typedef enum Axis {
    HORIZONTAL,
    VERTICAL,
} Axis;

extern const char* DIRECTIONS_STR[4];

extern const Vector2D DIRECTIONS_VECT[4];

extern const Axis DIRECTION_TO_AXIS[4];


int Geom_manhattan_distance(Point a, Point b);

Vector2D Vector2D_add(Vector2D v1, Vector2D v2);

Vector2D Vector2D_sub(Vector2D v1, Vector2D v2);

Vector2D Vector2D_mul(Vector2D v, double scalar);

Vector2D Vector2D_div(Vector2D v, double scalar);

double Vector2D_norm(Vector2D v);

Vector2D Vector2D_normalize(Vector2D v);

double Vector2D_dot(Vector2D v1, Vector2D v2);

#endif
