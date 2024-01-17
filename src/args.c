#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <MLV/MLV_all.h>

static void print_help(int exit_code) {
    printf("Usage: ./build/td [OPTIONS]\n");
    printf("Options:\n");
    printf("  -w, --window=WIDTHxHEIGHT  Set window size\n");
    printf("  -f, --fullscreen           Set fullscreen mode\n");
    printf("  -h, --help                 Print this help\n");
    exit(exit_code);
}

Args parse_args(int argc, char *argv[]) {
    static Args args = {
        .win = {
            .size = {800, 480},
            .fullscreen = false,
            .fps = 120,
        },
    };
    static struct option long_options[] = {
        {"window",        required_argument, NULL, 'w'},
        {"fullscreen",    no_argument,       NULL, 'f'},
        {"help",          no_argument,       NULL, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    char c;

    while ((c = getopt_long(argc, argv, "w:fh", long_options,
                       &option_index)) != -1) {
        switch (c) {
        case 'w':
            if (sscanf(optarg, "%lfx%lf", &args.win.size.width, &args.win.size.height) != 2 ||
                args.win.size.width < 500 || args.win.size.height < 500
            ) {
                fprintf(stderr, "Window size must be greater than 500x500\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'f':
            args.win.fullscreen = true;
            break;
        case 'h':
            print_help(EXIT_SUCCESS);
        case '?':
        default:
            print_help(EXIT_FAILURE);
        }
    }

    if (args.win.fullscreen) {
        args.win.size.width = MLV_get_desktop_width();
        args.win.size.height = MLV_get_desktop_height();
    }

    return args;
}
