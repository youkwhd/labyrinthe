#ifndef __LABYRINTHE_ARGS_H
#define __LABYRINTHE_ARGS_H

#include "coordinate.h"

typedef struct args_t {
    int maze_width, maze_height;
    coordinate_t starting_point, ending_point;
} args_t;

void args_parse(args_t *args, int argc, char **argv);

#endif // __LABYRINTHE_ARGS_H