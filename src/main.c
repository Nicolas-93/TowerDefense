#include <MLV/MLV_all.h>
#include <stdbool.h>
#include "timer.h"
#include "grid.h"
#include "args.h"
#include "land.h"
#include "image.h"
#include "event.h"
#include "game.h"

Args ARGS = {
    .win = {
        .size = {500, 400},
        .fullscreen = true,
        .fps = 60,
    },
};

int main(int argc, char const *argv[]) {

    if (ARGS.win.fullscreen) {
        ARGS.win.size.width = MLV_get_desktop_width();
        ARGS.win.size.height = MLV_get_desktop_height();
    }
    int seed = time(NULL);
    srand(seed);
    fprintf(stderr, "seed=%d\n", seed);

    MLV_create_window("", "", ARGS.win.size.width, ARGS.win.size.height);
    MLV_change_frame_rate(ARGS.win.fps);
    Clock_update();
    // Image_load_all();

    Game game;
    Game_new(&game, ARGS.win.size);

    while (true) {
        Clock_update();
        Event_pop_event();
        MLV_clear_window(MLV_COLOR_GRAY50);
        Game_process_event(&game);

        Game_update(&game);

        Game_draw(&game);
        MLV_update_window();

        MLV_delay_according_to_frame_rate();
    }

    return EXIT_SUCCESS;

}
