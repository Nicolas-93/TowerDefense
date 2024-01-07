#include "timer.h"

static Clock CURRENT_CLOCK = {0};

void Clock_update(void) {
    timespec_get(&CURRENT_CLOCK.current, TIME_UTC);
}

Timer Timer_new_ms(uint64_t milliseconds) {
    struct timespec now = CURRENT_CLOCK.current;

    return (Timer) {
        .future = {
            .tv_sec  = now.tv_sec  +  milliseconds / 1000,
            .tv_nsec = now.tv_nsec + (milliseconds % 1000) * 1e6,
        }
    };
}

bool Timer_is_over(const Timer* self) {
    struct timespec now = CURRENT_CLOCK.current;

    return (now.tv_sec == self->future.tv_sec ? now.tv_nsec >= self->future.tv_nsec :
                                                now.tv_sec > self->future.tv_sec);
}

int64_t Timer_get_difference(const Timer* self) {
    struct timespec now = CURRENT_CLOCK.current;

    return (
        (self->future.tv_sec - now.tv_sec)   * 1e3 +
        (self->future.tv_nsec - now.tv_nsec) / 1e6
    );
}
