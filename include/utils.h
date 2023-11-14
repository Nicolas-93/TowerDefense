#ifndef UTILS_H
#define UTILS_H

/**
 * @brief clamp a value between a min and a max
 * 
 */
#define clamp(value, min, max) ((value) < (min) ? (min) : (((value) > (max) ? (max) : (value))))

/**
 * @brief Minimum of two values
 * 
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define ABS(a) ((a) < 0 ? -(a) : (a))

double rand_double(double n);

double uniform(double a, double b);

int weighted_selection(int arrsize, int arr[arrsize]);

#endif
