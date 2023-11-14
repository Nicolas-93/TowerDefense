#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const char* DIRECTIONS_STR[4] = {
    [LEFT]  = "LEFT",
    [RIGHT] = "RIGHT",
    [UP]    = "UP",
    [DOWN]  = "DOWN",
};

double rand_double(double n) {
    return (double)rand() / (double)(RAND_MAX / n);
}

double uniform(double a, double b) {
    return a + (b - a) * rand_double(1);
}

int weighted_selection(int arrsize, int arr[arrsize]) {
    int cumulative[arrsize];
    memcpy(cumulative, arr, arrsize * sizeof(int));

    for (int i = 1; i < arrsize; ++i) {
        cumulative[i] += cumulative[i - 1];
    }

    int total = cumulative[arrsize - 1];
    // assert(total > 0);
    if (total == 0) {
        return 0;
    }
    int rnd = rand() % total;

    for (int i = 0; i < arrsize; ++i) {
        if (rnd < cumulative[i])
            return i;
    }
    return -1;
}
