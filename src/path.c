#include "path.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#define MIN_DIST 3

static const Point DIRECTIONS[4] = {
    [LEFT]  = {-1,  0},
    [RIGHT] = { 1,  0}, 
    [UP]    = { 0, -1},
    [DOWN]  = { 0,  1},
};

static void _Path_etendues_init(Path* self, Point start, int etendues[4]) {
    etendues[LEFT]  = start.x - MIN_DIST;
    etendues[RIGHT] = self->width  - start.x - MIN_DIST;
    etendues[DOWN]  = self->height - start.y - MIN_DIST;
    etendues[UP]    = start.y - MIN_DIST;
}

/*
static Direction _Path_new_turn_equal(Direction dir) {
    if (dir == RIGHT || dir == LEFT)
        return round(uniform(UP, DOWN));
    
    if (dir == UP || dir == DOWN)
        return round(uniform(RIGHT, LEFT));
}*/

static Direction _Path_new_turn(Direction old_dir, int etendues[4]) {
    int new_dirs[2];

    if (old_dir == RIGHT || old_dir == LEFT) {
        new_dirs[0] = etendues[UP];
        new_dirs[1] = etendues[DOWN];
        return weighted_selection(2, new_dirs) + UP;
    }
    
    else /*if (old_dir == UP || old_dir == DOWN)*/ {
        new_dirs[0] = etendues[RIGHT];
        new_dirs[1] = etendues[LEFT];
        return weighted_selection(2, new_dirs) + RIGHT;
    }

}

static bool _Path_is_outside(Path* self, Point a) {
    return (
        a.x < 0 || a.x >= self->width ||
        a.y < 0 || a.y >= self->height
    );
}

static void _Path_etendues(Path* self, Point start, int etendues[4]) {

    for (int i = 0; i < 4; ++i) {
        int etendue = 0;
        Point next = start;
        while (!_Path_is_outside(self, next)) {
            next = Vector2D_add(next, DIRECTIONS[i]);
            if (self->arr2d[(int) next.y][(int) next.x] != 0) {
                break;
            }
            etendue++;
        }
        etendues[i] = MAX(etendue - MIN_DIST, 0);
    }
}

static int _Path_random_sum(int etendue) {
    int sum = 0;
    for (int i = 0; i < etendue; ++i) {
        sum += uniform(0, 1) < 0.75;
    }
    return sum;
}

static Point _Path_add_segment(Path* self, Point start, Direction dir, int seg_size) {
    Point next = start;
    for (int i = 0; i < seg_size; ++i) {
        next = Vector2D_add(next, DIRECTIONS[dir]);
        self->arr2d[(int) next.y][(int) next.x] = PATH_CASE;
    }
    return next;
}

/**
 * @brief Generate a path in the land
 * 
 * @param self 
 */
static void _Path_generate_path(Path* self) {

    int nb_turns = 0;
    int path_len = 0;

    // srand(127);
    srand(0);

    while (nb_turns < 7 && path_len < 75) {
        memset(self->arr2d, 0, sizeof(PathCase) * self->width * self->height);

        Point start = (Point) {
            .x = (int) uniform(MIN_DIST, self->width - MIN_DIST),
            .y = (int) uniform(MIN_DIST, self->height - MIN_DIST),
        };

        int etendues[4] = {0};
        _Path_etendues_init(self, start, etendues);
        Direction dir = weighted_selection(4, etendues);

        while (etendues[dir] > 2) {
            int add = MAX(_Path_random_sum(etendues[dir]), MIN_DIST);
            start = _Path_add_segment(self, start, dir, add);
            Path_print(self);
            path_len += add;

            _Path_etendues(self, start, etendues);
            dir = _Path_new_turn(dir, etendues);
            nb_turns++;
        }
    }
}

void Path_print(Path* self) {
    printf("***************\n");
    for (int i = 0; i < self->height; ++i) {
        for (int j = 0; j < self->width; ++j) {
            char is_path = self->arr2d[i][j] == PATH_CASE ? ' ' : '0';
            fprintf(stderr, "%c ", is_path);
        }
        fprintf(stderr, "\n");
    }
    printf("***************\n");
}


void Path_new(Path* self) {
    *self = (Path) {
        .width = 28,
        .height = 22,
    };
    _Path_generate_path(self);
}
