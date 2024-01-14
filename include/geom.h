#ifndef GEOM_H
#define GEOM_H

#include <stdbool.h>

typedef struct {
    double x;
    double y;
} Vector2D, Point;

typedef struct Size {
    double width;
    double height;
} Size;

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
    UP      = 0,
    DOWN    = 1,
    RIGHT   = 2,
    LEFT    = 3,
} Direction;

typedef enum Axis {
    HORIZONTAL,
    VERTICAL,
} Axis;

extern const char* DIRECTIONS_STR[4];

extern const Vector2D DIRECTIONS_VECT[4];

extern const Axis DIRECTION_TO_AXIS[4];

/**
 * @brief Manhattan distance between two points
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int Geom_manhattan_distance(Point a, Point b);

/**
 * @brief Check if a point is on a segment
 * 
 * @param cell Point to check
 * @param p1 Segment's first point
 * @param p2 Segment's second point
 * @return true 
 * @return false 
 */
bool Point_on_segment(Point cell, Point p1, Point p2);

bool Point_on_circle(Point pos, Point center, double radius);

Vector2D Vector2D_new(double x, double y);

Vector2D Vector2D_new_from_value(double value);

bool Vector2D_equals(Vector2D v1, Vector2D v2);

Vector2D Vector2D_add(Vector2D v1, Vector2D v2);

Vector2D Vector2D_sub(Vector2D v1, Vector2D v2);

Vector2D Vector2D_mul(Vector2D v, double scalar);

Vector2D Vector2D_div(Vector2D v, double scalar);

double Vector2D_norm(Vector2D v);

double Vector2D_normalize(Vector2D* v);

double Vector2D_dist(Point a, Point b);

double Vector2D_dot(Vector2D v1, Vector2D v2);

bool Rect_contains(Rect rect, Point p);

#endif
