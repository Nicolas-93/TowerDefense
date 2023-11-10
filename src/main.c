#define _GNU_SOURCE
#include <MLV/MLV_all.h>
#include <stdbool.h>
#include <time.h>
#include "grid.h"
#include "args.h"
#include "land.h"

static Args ARGS = {
    .win = {
        .width = 1000,
        .height = 800,
    }
};

int main(int argc, char const *argv[]) {

    MLV_create_window("", "", ARGS.win.width, ARGS.win.height);

    Grid window_grid;
    Land land;

    /*
    struct timespec end_time, start_time;
    double elapsed = 0;
    double extratime = 0;
    */
    Grid_new(
        &window_grid, 10, 6, 0.95,
        NULL,
        (Rect) {.ax = 0, .ay = 0, .bx = ARGS.win.width, .by = ARGS.win.height},
        false,
        MLV_COLOR_WHITE, MLV_COLOR_RED
    );

    Land_new(&land, &window_grid, 28, 22);
    Path_print(&land.path);

    /**
    while (true) {
        const double fps = 60;
        const double frame_time = 1.0 / fps * 1000;
        clock_gettime(CLOCK_REALTIME, &start_time);

        Land_anim(&land);

        Grid_draw_lines(&window_grid);
        Grid_draw_lines(&land.grid);
        MLV_update_window();

        clock_gettime(CLOCK_REALTIME, &end_time);

        elapsed = (end_time.tv_sec - start_time.tv_sec) * 1e3 + (end_time.tv_nsec - start_time.tv_nsec) / 1e6;
        extratime = frame_time - elapsed;

        if (extratime > 0) {
            MLV_wait_milliseconds(extratime);
        }
    }*/

    return EXIT_SUCCESS;

}
