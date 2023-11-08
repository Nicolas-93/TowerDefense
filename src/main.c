#include <MLV/MLV_all.h>
#include "grid.h"

int main(int argc, char const *argv[])
{
    MLV_create_window("", "", 800, 500);
    Rect r = {.ax = 0, .ay = 0, .bx = 800, .by = 500};

    Grid* grid = Grid_new(100, 10, 0.8, NULL, r, 1, MLV_COLOR_WHITE, MLV_COLOR_RED);
    Grid_draw_lines(grid);
    MLV_update_window();
    MLV_wait_seconds(60);

    return 0;
}
