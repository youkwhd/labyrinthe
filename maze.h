#ifndef __LABYRINTHE_MAZE_H
#define __LABYRINTHE_MAZE_H

#include <stdint.h>

typedef enum maze_node_t {
    MAZE_NODE_DIR_NONE = 0,
    MAZE_NODE_DIR_BLOCKED,
    MAZE_NODE_DIR_UP,
    MAZE_NODE_DIR_RIGHT,
    MAZE_NODE_DIR_DOWN,
    MAZE_NODE_DIR_LEFT,
} maze_node_t;

typedef struct maze_t {
    uint16_t height, width;
    maze_node_t **body;
} maze_t;

void maze_init(maze_t *maze, uint16_t height, uint16_t width);
void maze_generate(maze_t *maze);
void maze_cleanup(maze_t *maze);

#endif // __LABYRINTHE_MAZE_H
