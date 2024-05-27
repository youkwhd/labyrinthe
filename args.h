#ifndef __LABYRINTHE_ARGS_H
#define __LABYRINTHE_ARGS_H

#include <stdbool.h>

#include "coordinate.h"

typedef struct args_t {
    bool use_gui;
    int maze_width, maze_height;
    coordinate_t starting_point, ending_point;
} args_t;

void args_parse(args_t *args, int argc, char **argv);

#endif // __LABYRINTHE_ARGS_H
