#ifndef TIMER_H
#define TIMER_H
#define _DEFAULT_SOURCE
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Timer {
    struct timespec future;
} Timer;

/**
 * @brief Create a new Timer object.
 * 
 * @param milliseconds 
 * @return Timer 
 */
Timer Timer_new_ms(uint64_t milliseconds);

/**
 * @brief Check if the timer is over.
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool Timer_is_over(const Timer* self);

/**
 * @brief Get time difference in milliseconds.
 * If the timer is over, return a negative value
 * (time that has passed since the timer ended).
 * else return a positive value (remaining time).
 * 
 * @param self 
 * @return uint64_t 
 */
int64_t Timer_get_difference(const Timer* self);

#endif
