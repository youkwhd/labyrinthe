#ifndef __LABYRINTHE_MAZE_H
#define __LABYRINTHE_MAZE_H

#include "matrix.h"

typedef struct maze_t {
    matrix_t body;
} maze_t;

void maze_init(maze_t *maze);

#endif // __LABYRINTHE_MAZE_H
