#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <stdint.h>

typedef struct Timer {
    struct timeval future;
} Timer;

/**
 * @brief Create a new Timer object.
 * 
 * @param milliseconds 
 * @return Timer 
 */
Timer Timer_new_ms(uint32_t milliseconds);

/**
 * @brief Check if the timer is over.
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool Timer_is_over(const Timer* self);

/**
 * @brief Get the remaining time in milliseconds.
 * 
 * @param self 
 * @return uint32_t 
 */
uint32_t Timer_get_remaining_time(const Timer* self);

#endif
