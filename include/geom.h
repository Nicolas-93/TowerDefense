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

/**
 * @brief Directions as strings
 * 
 */
extern const char* DIRECTIONS_STR[4];

/**
 * @brief Directions vectors
 * 
 * LEFT = {-1, 0}
 * RIGHT = {1, 0}
 * UP = {0, -1}
 * DOWN = {0, 1}
 * 
 */
extern const Vector2D DIRECTIONS_VECT[4];

/**
 * @brief Convert a direction to an axis
 * 
 */
extern const Axis DIRECTION_TO_AXIS[4];

/**
 * @brief Manhattan distance between two points
 * (number of cells to travel to go from a to b)
 * 
 * @param a First point
 * @param b Second point
 * @return int Manhattan distance between the two points
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

/**
 * @brief Check if a point is on a circle
 * 
 * @param pos Point to check
 * @param center Circle's center
 * @param radius Ciclle's radius
 * @return true Point is on the circle
 * @return false Point is not on the circle
 */
bool Point_on_circle(Point pos, Point center, double radius);

/**
 * @brief Create a new Vector2D object
 * 
 * @param x 
 * @param y 
 * @return Vector2D 
 */
Vector2D Vector2D_new(double x, double y);

/**
 * @brief Create a new Vector2D object with the same value for x and y
 * 
 * @param value Value to assign to x and y
 * @return Vector2D 
 */
Vector2D Vector2D_new_from_value(double value);

/**
 * @brief Check if two vectors or points are equals
 * 
 * @param v1 First vector/point
 * @param v2 Second vector/point
 * @return true Both vectors/points are equals
 * @return false Both vectors/points are different
 */
bool Vector2D_equals(Vector2D v1, Vector2D v2);

/**
 * @brief Add two vectors or points
 * 
 * @param v1 First vector/point
 * @param v2 Second vector/point
 * @return Vector2D Addition of the two vectors/points
 */
Vector2D Vector2D_add(Vector2D v1, Vector2D v2);

/**
 * @brief Substract two vectors or points
 * 
 * @param v1 First vector/point
 * @param v2 Second vector/point
 * @return Vector2D Substraction of the two vectors/points
 */
Vector2D Vector2D_sub(Vector2D v1, Vector2D v2);

/**
 * @brief Multiply a vector or point by a scalar
 * 
 * @param v Vector/point
 * @param scalar Scalar
 * @return Vector2D Multiplication of the vector/point by the scalar
 */
Vector2D Vector2D_mul(Vector2D v, double scalar);

/**
 * @brief Divide a vector or point by a scalar
 * 
 * @param v Vector/point
 * @param scalar Scalar
 * @return Vector2D Division of the vector/point by the scalar
 */
Vector2D Vector2D_div(Vector2D v, double scalar);

/**
 * @brief Get the norm of a vector
 * 
 * @param v Vector
 * @return double Norm of the vector
 */
double Vector2D_norm(Vector2D v);

/**
 * @brief Normalize a vector
 * 
 * @param v Vector
 * @return double Norm of the vector
 */
double Vector2D_normalize(Vector2D* v);

/**
 * @brief Get the distance between two points
 * 
 * @param a First point
 * @param b Second point
 * @return double Distance between the two points
 */
double Vector2D_dist(Point a, Point b);

/**
 * @brief Get the dot product of two vectors
 * 
 * @param v1 First vector
 * @param v2 Second vector
 * @return double Dot product of the two vectors
 */
double Vector2D_dot(Vector2D v1, Vector2D v2);

/**
 * @brief Check if a point is inside a rectangle
 * Right side and bottom side are not included
 * 
 * @param rect Rectangle to check
 * @param p Point to check
 * @return true if the point is inside the rectangle
 * @return false if the point is outside the rectangle
 */
bool Rect_contains(Rect rect, Point p);

/**
 * @brief Get the rectangle's width
 * 
 * @param rect 
 * @return double 
 */
double Rect_get_width(Rect rect);

/**
 * @brief Get the rectangle's height
 * 
 * @param rect 
 * @return double 
 */
double Rect_get_height(Rect rect);

#endif
