#ifndef __LABYRINTHE_MAZE_H
#define __LABYRINTHE_MAZE_H

#include <stdint.h>

typedef enum maze_node_t {
    MAZE_NODE_DIR_INVALID = -1,
    MAZE_NODE_DIR_NONE = 0,
    MAZE_NODE_DIR_BLOCKED = 1,
    MAZE_NODE_DIR_UP = 1 << 2,
    MAZE_NODE_DIR_RIGHT = 1 << 3,
    MAZE_NODE_DIR_DOWN = 1 << 4,
    MAZE_NODE_DIR_LEFT = 1 << 5,
} maze_node_t;

typedef struct maze_t {
    uint16_t height, width;
    maze_node_t **body;
} maze_t;

void maze_init(maze_t *maze, uint16_t height, uint16_t width);
void maze_generate(maze_t *maze);
void maze_cleanup(maze_t *maze);

#endif // __LABYRINTHE_MAZE_H
