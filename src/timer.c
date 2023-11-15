#include "timer.h"

Timer Timer_new_ms(uint64_t milliseconds) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    return (Timer) {
        .future = {
            .tv_sec = now.tv_sec + milliseconds / 1e3,
            .tv_nsec = now.tv_nsec + (milliseconds % 1000) * 1e6,
        }
    };
}

bool Timer_is_over(const Timer* self) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return (now.tv_sec == self->future.tv_sec ? now.tv_nsec >= self->future.tv_nsec :
                                                now.tv_sec > self->future.tv_sec);
}

int64_t Timer_get_difference(const Timer* self) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return (
        (self->future.tv_sec - now.tv_sec)   * 1e3 +
        (self->future.tv_nsec - now.tv_nsec) / 1e6
    );
}
