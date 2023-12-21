#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Cast a 1D array into a 2D VLA
 * This method avoid to create getters and setters,
 * in order to access an array as a 2D array.
 */
#define DECLARE_2D_VLA(name, typename, arr_ptr, width) \
    typename (*name)[width] = (typename (*)[width]) arr_ptr;
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

/**
 * @brief Maximum of two values
 * 
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * @brief Absolute value
 * 
 */
#define ABS(a) ((a) < 0 ? -(a) : (a))

/**
 * @brief Return a random double in the range [0, n[
 * 
 * @param n 
 * @return double 
 */
double rand_double(double n);

/**
 * @brief Return a random double in the range [a, b[
 * 
 * @param a 
 * @param b 
 * @return double 
 */
double uniform(double a, double b);

/**
 * @brief Return a random index of the array,
 * weighted by the values of the array
 * 
 * @param arrsize Array size 
 * @param arr Array of integers (weights)
 * @return int 
 */
int weighted_selection(int arrsize, const int arr[arrsize]);

#endif
