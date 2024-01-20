#include <MLV/MLV_all.h>
#include <stdbool.h>
#include "timer.h"
#include "grid.h"
#include "args.h"
#include "land.h"
#include "image.h"
#include "event.h"
#include "game.h"

static __sig_atomic_t RUNNING = true;
void execute_at_exit(void* data) { RUNNING = false; }

int main(int argc, char *argv[]) {

    Args args = parse_args(argc, argv);
    Game game;
    Error err = 0;

    int seed = time(NULL);
    srand(seed);
    fprintf(stderr, "seed=%d\n", seed);

    MLV_execute_at_exit(execute_at_exit, &game);
    MLV_create_window("", "", args.win.size.width, args.win.size.height);
    MLV_change_frame_rate(args.win.fps);
    Clock_update();

    if ((err = Image_load_all()) < 0 ||
        (err = Game_new(&game, args.win.size)) < 0) {
        fprintf(stderr, "Error: %s\n", Error_to_string(err));
        return EXIT_FAILURE;
    }

    while (RUNNING) {
        Clock_update();
        Event_pop_event();
        MLV_clear_window(MLV_COLOR_GRAY50);
        Game_process_event(&game);

        Game_update(&game);

        Game_draw(&game);
        MLV_update_window();

        MLV_delay_according_to_frame_rate();
    }

    Game_free(&game);
    Image_free();
    MLV_free_window();

    return EXIT_SUCCESS;

}
