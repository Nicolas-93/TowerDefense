#include <MLV/MLV_all.h>
#include <stdbool.h>
#include "timer.h"
#include "grid.h"
#include "args.h"
#include "land.h"
#include "image.h"
#include "event.h"
#include "game.h"

int main(int argc, char *argv[]) {

    Args args = parse_args(argc, argv);

    int seed = time(NULL);
    srand(seed);
    fprintf(stderr, "seed=%d\n", seed);

    MLV_create_window("", "", args.win.size.width, args.win.size.height);
    MLV_change_frame_rate(args.win.fps);
    Clock_update();
    Image_load_all();

    Game game;
    Game_new(&game, args.win.size);

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
