#include "path.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MIN_DIST 3

#define MIN_BORDER 3

static const Vector2D DIRECTIONS[4] = {
    [LEFT]  = {-1,  0},
    [RIGHT] = { 1,  0}, 
    [UP]    = { 0, -1},
    [DOWN]  = { 0,  1},
};

typedef enum Axis {
    HORIZONTAL,
    VERTICAL,
} Axis;

static const Axis DIRECTION_TO_AXIS[4] = {
    [LEFT]  = HORIZONTAL,
    [RIGHT] = HORIZONTAL,
    [UP]    = VERTICAL,
    [DOWN]  = VERTICAL,
};

static bool _Path_case_is_outside(Path* self, Point a) {
    return (
        a.x < MIN_BORDER || a.x >= (self->width - MIN_BORDER) ||
        a.y < MIN_BORDER || a.y >= (self->height - MIN_BORDER)
    );
}

static bool _Path_case_in_segment(Point cell, Point p1, Point p2) {
    if (p1.x == p2.x) {
        return cell.x == p1.x && cell.y >= MIN(p1.y, p2.y) && cell.y <= MAX(p1.y, p2.y);
    }
    if (p1.y == p2.y) {
        return cell.y == p1.y && cell.x >= MIN(p1.x, p2.x) && cell.x <= MAX(p1.x, p2.x);
    }
    return false;
}

static bool _Path_case_is_distant(Path* self, Point origin, Direction dir) {
    int x_start = origin.x - MIN_DIST;
    int x_end   = origin.x + MIN_DIST;
    int y_start = origin.y - MIN_DIST;
    int y_end   = origin.y + MIN_DIST;

    if (ArrayList_get_length(&self->waypoints) <= 1) {
        return true;
    }

    for (int i = y_start; i < y_end; ++i) {
        for (int j = x_start; j < x_end; ++j) {
            Point current = {
                .x = j,
                .y = i,
            };
            if (Geom_manhattan_distance(origin, current) >= MIN_DIST) {
                continue;
            }
            if (origin.x == j && origin.y == i) {
                continue;
            }
            if (ArrayList_get_length(&self->waypoints) >= 2 && _Path_case_in_segment(
                current, * (Point*) ArrayList_get(&self->waypoints, -1), * (Point*) ArrayList_get(&self->waypoints, -2)
            )) {
                continue;
            }
            if (self->arr2d[i][j] != PATH_CASE_EMPTY) {
                return false;
            }
        }
    }

    return true;
}

static void _Path_etendues_init(Path* self, Point start, int etendues[4]) {
    etendues[UP]    = start.y - (2 * MIN_DIST);
    etendues[DOWN]  = self->height - start.y - (2 * MIN_DIST);
    etendues[RIGHT] = self->width  - start.x - (2 * MIN_DIST);
    etendues[LEFT]  = start.x - (2 * MIN_DIST);
}

/**
 * @brief Calcule les étendues pour deux directions
 * 
 * @param self Path
 * @param start 
 * @param axis Axis
 * @param etendues dest array for each direction
 */
static void _Path_etendues_axis(Path* self, Point start, Axis axe, int etendues[4]) {
    Direction offset = axe == VERTICAL ? UP : RIGHT;

    for (Direction dir = offset; dir < 2 + offset; ++dir) {
        Vector2D vdir = DIRECTIONS[dir];

        int length = 0;
        for (
            Point pos = Vector2D_add(start, vdir);

            !_Path_case_is_outside(self, pos)
                && _Path_case_is_distant(self, pos, dir);

            pos = Vector2D_add(pos, vdir)
        ) {
            length++;
        }
        etendues[dir] = length;
    }
}

static int _Path_random_sum(int etendue) {
    int sum = 0;
    for (int i = 0; i < etendue; ++i) {
        sum += uniform(0, 1) < 0.75;
    }
    return sum;
}

/**
 * @brief Adds a new segment to the path, starting after ``start`` point.
 * 
 * @param self Path
 * @param start Current position
 * @param dir Direction
 * @param seg_size Number of cells to add
 * @return Point End of the path
 */
static Point _Path_add_segment(Path* self, Point start, Direction dir, int seg_size) {
    Point next = start;
    for (int i = 0; i < seg_size; ++i) {
        next = Vector2D_add(next, DIRECTIONS[dir]);
        self->arr2d[(int) roundl(next.y)][(int) roundl(next.x)] = PATH_CASE;
    }
    return next;
}

static Direction _Path_new_turn(Axis axis, int etendues[4]) {
    int new_dirs[2];

    if (axis == VERTICAL) {
        return weighted_selection(2, etendues + UP) + UP;
    }
    
    else /*if (axis == HORIZONTAL)*/ {
        return weighted_selection(2, etendues + RIGHT) + RIGHT;
    }
}

/**
 * @brief Generate a path in the land
 * 
 * @param self 
 */
static void _Path_generate_path(Path* self) {

    int nb_turns = 0;
    int path_len = 0;
    int nb_attempts = 0;

    int seed = time(NULL);
    srand(seed);
    fprintf(stderr, "seed=%d\n", seed);
    ArrayList_init(&self->waypoints, sizeof(Point), 20, NULL);

    while (nb_turns < 8 || path_len < 75) {
        nb_attempts++;

        nb_turns = 0;
        path_len = 0;
        memset(self->arr2d, PATH_CASE_EMPTY, self->height * self->width * sizeof(PathCase));
        ArrayList_clear(&self->waypoints);

        Point start = (Point) {
            .x = (int) uniform(3, self->width - 3),
            .y = (int) uniform(3, self->height - 3),
        };

        self->arr2d[(int) start.y][(int) start.x] = PATH_CASE;
        ArrayList_append(&self->waypoints, &start);
        // Path_print(self);

        int etendues[4] = {0};
        _Path_etendues_init(self, start, etendues);
        Direction dir = weighted_selection(4, etendues);

        Axis axis = DIRECTION_TO_AXIS[dir];

        while (etendues[dir] > 2) {
            int seg_size = MAX(_Path_random_sum(etendues[dir]), 3);
            path_len += seg_size;
            start = _Path_add_segment(self, start, dir, seg_size);
            ArrayList_append(&self->waypoints, &start);
            // Path_print(self);
            
            memset(etendues, 0, sizeof(int) * 4);
            axis ^= 1; // Opposite axis
            _Path_etendues_axis(self, start, axis, etendues);
            
            dir = _Path_new_turn(axis, etendues);
            nb_turns++;
        }
    }
    Path_print(self);
    fprintf(stderr, "path_len=%d, nb_turns=%d, seed=%d, nb_attempts=%d\n", path_len, nb_turns, seed, nb_attempts);
}

void Path_print(Path* self) {
    fprintf(stderr, "***************\n");

    fprintf(stderr, "  ");
    for (int i = 0; i < self->width; ++i) {
        fprintf(stderr, "%2d", i);
    }
    fprintf(stderr, "\n");

    for (int i = 0; i < self->height; ++i) {
        fprintf(stderr, "%2d ", i);
        for (int j = 0; j < self->width; ++j) {
            char is_path = self->arr2d[i][j] == PATH_CASE ? ' ' : '0';
            fprintf(stderr, "%1c ", is_path);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "***************\n");
}


void Path_new(Path* self) {
    *self = (Path) {
        .width = 28,
        .height = 22,
    };
    _Path_generate_path(self);
}
