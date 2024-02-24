#ifndef __LABYRINTHE_MAZE_H
#define __LABYRINTHE_MAZE_H

#include "matrix.h"

typedef enum maze_node_t {
    MAZE_NODE_DIR_NONE = 0,
    MAZE_NODE_DIR_BLOCKED,
    MAZE_NODE_DIR_LEFT,
    MAZE_NODE_DIR_RIGHT,
    MAZE_NODE_DIR_UP,
    MAZE_NODE_DIR_DOWN,
} maze_node_t;

typedef struct maze_t {
    matrix_t body;
} maze_t;

void maze_init(maze_t *maze);
void maze_cleanup(maze_t *maze);

#endif // __LABYRINTHE_MAZE_H
