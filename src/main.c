#include <MLV/MLV_all.h>
#include <stdbool.h>
#include <time.h>
#include "grid.h"
#include "args.h"
#include "land.h"

static Args ARGS = {
    .win = {
        .width = 500,
        .height = 400,
        .fps = 60,
    }
};

int main(int argc, char const *argv[]) {

    MLV_create_window("", "", ARGS.win.width, ARGS.win.height);
    MLV_change_frame_rate(ARGS.win.fps);

    Grid window_grid;
    Land land;

    Grid_new(
        &window_grid, 10, 6, 0.95,
        NULL,
        (Rect) {.ax = 0, .ay = 0, .bx = ARGS.win.width, .by = ARGS.win.height},
        false,
        MLV_COLOR_WHITE, MLV_COLOR_RED
    );

    Land_new(&land, &window_grid, 28, 22);
    Path_print(&land.path);

    while (true) {
        MLV_clear_window(MLV_COLOR_GRAY50);
        Land_anim(&land);

        Land_draw(&land);
        // Grid_draw_lines(&window_grid);
        MLV_update_window();

        MLV_delay_according_to_frame_rate();
    }

    return EXIT_SUCCESS;

}
